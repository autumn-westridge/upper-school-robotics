#define SIDES 5

int side = 0;
int sequence_lengths[SIDES] = {3, 4, 5, 2, 1};
int sequences[SIDES][MAX_SEQUENCE_LENGTH] = {
  {11, 0, 11, -1, -1},
  {2, 2, 2, 10, -1},
  {3, 4, 5, 6, 7},
  {1, 9, -1, -1, -1},
  {4, -1, -1, -1, -1}
};

void advanceSide() {
  side++;
  if (side == SIDES) {
    // Totally done!
  }
  else {
    SEQUENCE_LENGTH = sequence_lengths[side];
    memcpy(target_order, sequences[side], sizeof sequences[side]);
  }
}
