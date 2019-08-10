/** Reads a 1-5 byte int. */
public int readInt (boolean optimizePositive) {
   if (require(1) < 5) return readInt_slow(optimizePositive);
   int b = buffer[position++];
   int result = b & 0x7F;
   if ((b & 0x80) != 0) {
      byte[] buffer = this.buffer;
      b = buffer[position++];
      result |= (b & 0x7F) << 7;
      if ((b & 0x80) != 0) {
         b = buffer[position++];
         result |= (b & 0x7F) << 14;
         if ((b & 0x80) != 0) {
            b = buffer[position++];
            result |= (b & 0x7F) << 21;
            if ((b & 0x80) != 0) {
               b = buffer[position++];
               result |= (b & 0x7F) << 28;
            }
         }
      }
   }
   return optimizePositive ? result : ((result >>> 1) ^ -(result & 1));
}

private int readInt_slow (boolean optimizePositive) {
   // The buffer is guaranteed to have at least 1 byte.
   int b = buffer[position++];
   int result = b & 0x7F;
   if ((b & 0x80) != 0) {
      require(1);
      byte[] buffer = this.buffer;
      b = buffer[position++];
      result |= (b & 0x7F) << 7;
      if ((b & 0x80) != 0) {
         require(1);
         b = buffer[position++];
         result |= (b & 0x7F) << 14;
         if ((b & 0x80) != 0) {
            require(1);
            b = buffer[position++];
            result |= (b & 0x7F) << 21;
            if ((b & 0x80) != 0) {
               require(1);
               b = buffer[position++];
               result |= (b & 0x7F) << 28;
            }
         }
      }
   }
   return optimizePositive ? result : ((result >>> 1) ^ -(result & 1));
}