#ifndef CLOCK_H
#define CLOCK_H

#define CLOCK_SIZE 1536

const char clockFrame[CLOCK_SIZE] = "\
     ---- ----     \n\
   /           \\   \n\
                   \n\
 /               \\ \n\
         o         \n\
 \\               / \n\
                   \n\
   \\           /   \n\
     ---- ----     \n";

const char clockTicks[CLOCK_SIZE] = "\
    \\    |    /     \
                    \
  -             -   \
                    \
-        o        - \
                    \
  -             -   \
                    \
    /    |    \\     ";

const char hrHands[][CLOCK_SIZE] = {"\
                   \n\
                   \n\
         ^         \n\
         |         \n\
         o         \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
          /        \n\
         |         \n\
         o         \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
           _/      \n\
          /        \n\
         o         \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
            _      \n\
          _-       \n\
         o         \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
           __-     \n\
         o-        \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
            __     \n\
         o--       \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o--->     \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o--__     \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o-__      \n\
             -     \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o_        \n\
           -_      \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o         \n\
          \\_       \n\
            \\      \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o         \n\
         |         \n\
          \\        \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
         o         \n\
         |         \n\
         v         \n\
                   \n\
                   \n",
};

const char minHands[][CLOCK_SIZE] = {"\
         |         \n\
         |         \n\
         |         \n\
         |         \n\
         o         \n\
         |         \n\
                   \n\
                   \n\
                   \n",

"\
            /      \n\
           |       \n\
          /        \n\
         |         \n\
         o         \n\
        /          \n\
                   \n\
                   \n\
                   \n",

"\
              /    \n\
             /     \n\
           _/      \n\
          /        \n\
         o         \n\
        /          \n\
                   \n\
                   \n\
                   \n",

"\
                _  \n\
              _-   \n\
            _-     \n\
          _-       \n\
        _o         \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                 _ \n\
              __-  \n\
           __-     \n\
        _o-        \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                  _\n\
            ___--- \n\
        _o--       \n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
                   \n\
       --o---------\n\
                   \n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
        _          \n\
         o--___    \n\
               ---_\n\
                   \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
        _          \n\
         o-__      \n\
             -__   \n\
                -_ \n\
                   \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
        _          \n\
         o_        \n\
           -_      \n\
             -_    \n\
               -_  \n\
                   \n",

"\
                   \n\
                   \n\
                   \n\
        \\          \n\
         o         \n\
          \\_       \n\
            \\      \n\
             \\     \n\
              \\    \n",

"\
                   \n\
                   \n\
                   \n\
        \\          \n\
         o         \n\
         |         \n\
          \\        \n\
           |       \n\
            \\      \n",

"\
                   \n\
                   \n\
                   \n\
         |         \n\
         o         \n\
         |         \n\
         |         \n\
         |         \n\
         |         \n",
};

#endif //CLOCK_H
