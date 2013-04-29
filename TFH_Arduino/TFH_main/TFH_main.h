#ifndef TFH_H
#define TFH_H

#define BYTES_PER_MESSAGE 2
#define DESIRED_COLOR_INDEX 0
#define RELAY_DATA_INDEX 1

#define CURRENT_AROMA_MASK (1<<3)
#define DESIRED_AROMA_MASK (1<<2)
#define CURRENT_FAN_MASK (1<<1)
#define DESIRED_FAN_MASK (1)
#define CURRENT_BUBBLE_MASK (1<<5)
#define DESIRED_BUBBLE_MASK (1<<4)
#define CURRENT_IR_MASK (1<<7)
#define DESIRED_IR_MASK (1<<6)

#define HEAD 'r'
#define FOOT 'p'

#define DEBUG 0

#endif
