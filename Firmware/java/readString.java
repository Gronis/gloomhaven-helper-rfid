/** Reads the length and string of UTF8 characters, or null. This can read strings written by
 * {@link Output#writeString(String)} , {@link Output#writeString(CharSequence)}, and {@link Output#writeAscii(String)}.
 * @return May be null. */
public String readString () {
   int available = require(1);
   int b = buffer[position++];
   if ((b & 0x80) == 0) return readAscii(); // ASCII.
   // Null, empty, or UTF8.
   int charCount = available >= 5 ? readUtf8Length(b) : readUtf8Length_slow(b);
   switch (charCount) {
   case 0:
      return null;
   case 1:
      return "";
   }
   charCount--;
   if (chars.length < charCount) chars = new char[charCount];
   readUtf8(charCount);
   return new String(chars, 0, charCount);
}

private int readUtf8Length (int b) {
   int result = b & 0x3F; // Mask all but first 6 bits.
   if ((b & 0x40) != 0) { // Bit 7 means another byte, bit 8 means UTF8.
      byte[] buffer = this.buffer;
      b = buffer[position++];
      result |= (b & 0x7F) << 6;
      if ((b & 0x80) != 0) {
         b = buffer[position++];
         result |= (b & 0x7F) << 13;
         if ((b & 0x80) != 0) {
            b = buffer[position++];
            result |= (b & 0x7F) << 20;
            if ((b & 0x80) != 0) {
               b = buffer[position++];
               result |= (b & 0x7F) << 27;
            }
         }
      }
   }
   return result;
}

private int readUtf8Length_slow (int b) {
   int result = b & 0x3F; // Mask all but first 6 bits.
   if ((b & 0x40) != 0) { // Bit 7 means another byte, bit 8 means UTF8.
      require(1);
      byte[] buffer = this.buffer;
      b = buffer[position++];
      result |= (b & 0x7F) << 6;
      if ((b & 0x80) != 0) {
         require(1);
         b = buffer[position++];
         result |= (b & 0x7F) << 13;
         if ((b & 0x80) != 0) {
            require(1);
            b = buffer[position++];
            result |= (b & 0x7F) << 20;
            if ((b & 0x80) != 0) {
               require(1);
               b = buffer[position++];
               result |= (b & 0x7F) << 27;
            }
         }
      }
   }
   return result;
}

private void readUtf8 (int charCount) {
   byte[] buffer = this.buffer;
   char[] chars = this.chars;
   // Try to read 7 bit ASCII chars.
   int charIndex = 0;
   int count = Math.min(require(1), charCount);
   int position = this.position;
   int b;
   while (charIndex < count) {
      b = buffer[position++];
      if (b < 0) {
         position--;
         break;
      }
      chars[charIndex++] = (char)b;
   }
   this.position = position;
   // If buffer didn't hold all chars or any were not ASCII, use slow path for remainder.
   if (charIndex < charCount) readUtf8_slow(charCount, charIndex);
}

private void readUtf8_slow (int charCount, int charIndex) {
   char[] chars = this.chars;
   byte[] buffer = this.buffer;
   while (charIndex < charCount) {
      if (position == limit) require(1);
      int b = buffer[position++] & 0xFF;
      switch (b >> 4) {
      case 0:
      case 1:
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
         chars[charIndex] = (char)b;
         break;
      case 12:
      case 13:
         if (position == limit) require(1);
         chars[charIndex] = (char)((b & 0x1F) << 6 | buffer[position++] & 0x3F);
         break;
      case 14:
         require(2);
         chars[charIndex] = (char)((b & 0x0F) << 12 | (buffer[position++] & 0x3F) << 6 | buffer[position++] & 0x3F);
         break;
      }
      charIndex++;
   }
}

private String readAscii () {
   byte[] buffer = this.buffer;
   int end = position;
   int start = end - 1;
   int limit = this.limit;
   int b;
   do {
      if (end == limit) return readAscii_slow();
      b = buffer[end++];
   } while ((b & 0x80) == 0);
   buffer[end - 1] &= 0x7F; // Mask end of ascii bit.
   try {
      String value = new String(buffer, start, end - start, "ASCII");
      buffer[end - 1] |= 0x80;
      position = end;
      return value;
   } catch (UnsupportedEncodingException ex) {
      throw new RuntimeException();
   }
}

private String readAscii_slow () {
   position--; // Re-read the first byte.
   // Copy chars currently in buffer.
   int charCount = limit - position;
   if (charCount > chars.length) chars = new char[charCount * 2];
   char[] chars = this.chars;
   byte[] buffer = this.buffer;
   for (int i = position, ii = 0, n = limit; i < n; i++, ii++)
      chars[ii] = (char)buffer[i];
   position = limit;
   // Copy additional chars one by one.
   while (true) {
      require(1);
      int b = buffer[position++];
      if (charCount == chars.length) {
         char[] newChars = new char[charCount * 2];
         System.arraycopy(chars, 0, newChars, 0, charCount);
         chars = newChars;
         this.chars = newChars;
      }
      if ((b & 0x80) == 0x80) {
         chars[charCount++] = (char)(b & 0x7F);
         break;
      }
      chars[charCount++] = (char)b;
   }
   return new String(chars, 0, charCount);
}