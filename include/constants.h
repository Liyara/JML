#ifndef JML_CONSTANTS_H
#define JML_CONSTANTS_H

#define JML_EPSILON 0.000001L
#define JML_CUT     0.0000001L
#define JML_PI      0x3.243F6A8p+0L
#define JML_PIO2    0x3.243F6A8p-1L
#define JML_TAU     0x3.243F6A8p+1L
#define JML_E       0x0.93C467Ep+0L
#define JML_PHI     0x1.9E3779Bp+0L
#define SQRT2       0x1.6A09E66p+0L

enum {
    JML_LESS = -1,
    JML_EQUAL,
    JML_GREATER
};

enum {
    JML_ROUND_UP,
    JML_ROUND_DOWN
};

enum {
    JML_INTEGER_PART,
    JML_DECIMAL_PART
};

#endif // JML_CONSTANTS_H
