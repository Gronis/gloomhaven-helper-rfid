static public void write (Output output, GameState state) {
   output.writeInt(state.round, true);
   output.writeInt(state.scenarioNumber, true);
   output.writeInt(state.scenarioLevel, true);
   output.writeBoolean(state.trackStandees);
   output.writeBoolean(state.randomStandees);
   output.writeBoolean(state.elitesFirst);
   output.writeBoolean(state.expireConditions);
   output.writeBoolean(state.solo);
   output.writeBoolean(state.hideStats);
   output.writeBoolean(state.calculateStats);
   output.writeBoolean(state.canDraw);
   output.writeBoolean(state.needsShuffle);
   writeEnum(output, state.playerInit);
   writeEnumArray(output, state.attackModifiers);
   writeEnumOrNull(output, state.attackModifier1);
   writeEnumOrNull(output, state.attackModifier2);
   writeEnum(output, state.fire);
   writeEnum(output, state.ice);
   writeEnum(output, state.air);
   writeEnum(output, state.earth);
   writeEnum(output, state.light);
   writeEnum(output, state.dark);

   output.writeInt(state.abilityDecks.size, true);
   for (MonsterAbilityDeck deck : state.abilityDecks.values()) {
      output.writeInt(deck.id, true);
      output.writeBoolean(deck.shown);
      output.writeBoolean(deck.wasShown);
      for (int i = 0, n = writeArrayStart(output, deck.abilities); i < n; i++)
         output.writeInt(deck.abilities.get(i).id, true);
   }

   SnapshotArray<Actor> children = gloom.rows.getChildren();
   for (int i = 0, n = writeArrayStart(output, children); i < n; i++) {
      Actor actor = children.get(i);
      if (actor instanceof PlayerRow) {
         output.writeBoolean(true);
         writePlayerRow(output, (PlayerRow)actor);
      } else {
         output.writeBoolean(false);
         writeMonsterRow(output, (MonsterRow)actor);
      }
   }
}

static public GameState read (Input input) {
   GameState state = new GameState();
   state.round = input.readInt(true);
   state.scenarioNumber = input.readInt(true);
   state.scenarioLevel = input.readInt(true);
   state.trackStandees = input.readBoolean();
   state.randomStandees = input.readBoolean();
   state.elitesFirst = input.readBoolean();
   state.expireConditions = input.readBoolean();
   state.solo = input.readBoolean();
   state.hideStats = input.readBoolean();
   state.calculateStats = input.readBoolean();
   state.canDraw = input.readBoolean();
   state.needsShuffle = input.readBoolean();
   state.playerInit = readEnum(input, PlayerInit.values);
   readEnumArray(state.attackModifiers, input, AttackModifier.values);
   state.attackModifier1 = readEnumOrNull(input, AttackModifier.values);
   state.attackModifier2 = readEnumOrNull(input, AttackModifier.values);
   state.fire = readEnum(input, ElementState.values);
   state.ice = readEnum(input, ElementState.values);
   state.air = readEnum(input, ElementState.values);
   state.earth = readEnum(input, ElementState.values);
   state.light = readEnum(input, ElementState.values);
   state.dark = readEnum(input, ElementState.values);

   for (int i = 0, n = input.readInt(true); i < n; i++) {
      MonsterAbilityDeck deck = new MonsterAbilityDeck(findMonsterAbilityDeck(input.readInt(true)));
      state.abilityDecks.put(deck.id, deck);
      deck.shown = input.readBoolean();
      deck.wasShown = input.readBoolean();
      deck.abilities.clear();
      for (int ii = 0, nn = input.readInt(true); ii < nn; ii++)
         deck.abilities.add(monsterAbilities.get(input.readInt(true)));
   }

   Rows rows = gloom.rows;
   rows.clearChildren();
   gloom.playerRows.clear();
   gloom.monsterRows.clear();
   for (int i = 0, n = input.readInt(true); i < n; i++) {
      if (input.readBoolean()) {
         PlayerRow row = readPlayerRow(input);
         rows.addActor(row);
         gloom.playerRows.add(row);
      } else {
         MonsterRow row = readMonsterRow(input);
         rows.addActor(row);
         gloom.monsterRows.add(row);
      }
   }
   return state;
}

static private void writePlayerRow (Output output, PlayerRow row) {
   Player player = row.player;
   if (player.name.equals(player.characterClass.toString()))
      output.writeString(null);
   else
      output.writeString(player.name);
   writeEnum(output, player.characterClass);
   output.writeInt(player.xp, true);
   output.writeInt(player.hp, true);
   output.writeInt(player.hpMax, true);
   output.writeInt(player.level, true);
   output.writeInt(player.loot, true);
   output.writeInt(player.init, true);
   writeEnumArray(output, player.conditions);
   writeEnumArray(output, player.expiredConditions);
   writeEnumArray(output, player.currentTurnConditions);
   output.writeBoolean(player.exhausted);

   writeRow(output, row);
}

static private PlayerRow readPlayerRow (Input input) {
   Player player = new Player();
   player.name = input.readString();
   player.characterClass = readEnum(input, CharacterClass.values);
   if (player.name == null) player.name = player.characterClass.toString();
   player.xp = input.readInt(true);
   player.hp = input.readInt(true);
   player.hpMax = input.readInt(true);
   player.level = input.readInt(true);
   player.loot = input.readInt(true);
   player.init(input.readInt(true));
   readEnumArray(player.conditions, input, Condition.values);
   readEnumArray(player.expiredConditions, input, Condition.values);
   readEnumArray(player.currentTurnConditions, input, Condition.values);
   player.exhausted = input.readBoolean();

   PlayerRow row = new PlayerRow(player);
   readRow(input, row);
   return row;
}

static private void writeMonsterRow (Output output, MonsterRow row) {
   output.writeInt(-(row.data.id + 1), true);
   output.writeInt(row.level, true);
   output.writeBoolean(row.hasNormal);
   output.writeBoolean(row.hasElite);
   writeRow(output, row);
}

static private MonsterRow readMonsterRow (Input input) {
   MonsterData data = monsterDatas.get(input.readInt(true));
   MonsterRow row = new MonsterRow(data, input.readInt(true));
   row.hasNormal = input.readBoolean();
   row.hasElite = input.readBoolean();
   readRow(input, row);
   return row;
}

static private void writeRow (Output output, Row row) {
   output.writeBoolean(row.turnComplete);
   for (int i = 0, n = writeArrayStart(output, row.boxes); i < n; i++) {
      Monster monster = row.boxes.get(i).monster;
      output.writeInt(monster.number, true);
      writeEnum(output, monster.type);
      if (monster.type == MonsterType.summon) {
         writeEnum(output, monster.summonColor);
         output.writeInt(monster.summonMove, true);
         output.writeInt(monster.summonAttack, true);
         output.writeInt(monster.summonRange, true);
      }
      output.writeBoolean(monster.isNew);
      output.writeInt(monster.hp, true);
      output.writeInt(monster.hpMax, true);
      writeEnumArray(output, monster.conditions);
      writeEnumArray(output, monster.expiredConditions);
      writeEnumArray(output, monster.currentTurnConditions);
   }
}

static private void readRow (Input input, Row row) {
   row.setTurnComplete(input.readBoolean(), false);
   for (int ii = 0, nn = input.readInt(true); ii < nn; ii++) {
      Monster monster = new Monster();
      monster.number = input.readInt(true);
      monster.type = readEnum(input, MonsterType.values);
      if (monster.type == MonsterType.summon) {
         monster.summonColor = readEnum(input, SummonColor.values);
         monster.summonMove = input.readInt(true);
         monster.summonAttack = input.readInt(true);
         monster.summonRange = input.readInt(true);
      }
      monster.isNew = input.readBoolean();
      monster.hp = input.readInt(true);
      monster.hpMax = input.readInt(true);
      readEnumArray(monster.conditions, input, Condition.values);
      readEnumArray(monster.expiredConditions, input, Condition.values);
      readEnumArray(monster.currentTurnConditions, input, Condition.values);

      if (row instanceof MonsterRow) {
         MonsterRow monsterRow = (MonsterRow)row;
         monster.data = monsterRow.data;
         monster.stats = monsterRow.data.stats[monster.type.ordinal()][monsterRow.level];
      } else {
         monster.data = summonData;
         monster.stats = summonStats;
      }

      MonsterBox box = new MonsterBox(row, monster, row.monstersGroup.getScaleX());
      row.boxes.add(box);
      row.monstersGroup.addActor(box);
   }
}

static private void writeEnum (Output output, Enum value) {
   output.writeInt(value.ordinal(), true);
}

static private <T extends Enum> T readEnum (Input input, T[] values) {
   return values[input.readInt(true)];
}

static private void writeEnumOrNull (Output output, Enum value) {
   if (value == null)
      output.writeByte(0);
   else
      output.writeInt(value.ordinal() + 1, true);
}

static private <T extends Enum> T readEnumOrNull (Input input, T[] values) {
   int value = input.readInt(true);
   if (value == 0) return null;
   return values[value - 1];
}

static private int writeArrayStart (Output output, Array array) {
   int size = array.size;
   output.writeInt(size, true);
   return size;
}

static private void writeEnumArray (Output output, Array<? extends Enum> array) {
   int length = array.size;
   output.writeInt(length, true);
   if (length == 0) return;
   for (int i = 0, n = array.size; i < n; i++)
      writeEnum(output, array.get(i));
}

static private <T extends Enum> Array<T> readEnumArray (Array<T> array, Input input, T[] values) {
   int length = input.readInt(true);
   array.ensureCapacity(length);
   for (int i = 0; i < length; i++)
      array.add(readEnum(input, values));
   return array;
}