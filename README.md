# ice_timer
This is a silly project that uses the Cypress PSoC3 to settle a bet between a friend and I.

We were arguing about whether or not and ice cube would melt faster or slower when a fan is blowing
on it. Here are the two positions:

1. It will melt faster because the ice cube will cool the air immediately around it, but the fan
will blow away the cold air, replacing it with warmer air that will cause the ice to melt faster.

2. It will melt more slowly because moving air has a cooling effect on objects that it passes by.

We will test the competing hypotheses using the Cypress PSoC3 development kit (CY8KIT-030) and a
small structure built from a K'NEX set. 

The ice cube will be placed on a platfor, and a rod from the K'NEX set that is attached to a pivot,
so the rod will act as a movable arm that can rotate up and down. There will be a wire running down
the K'NEX rod. When the arm is in the UP position, it will close a contact connected to
one of the PSoC's GPOIs. When the contact is broken (the arm is dropped to rest on the ice cube), a
timer will start. As the ice cube melts, the arm will be slowly lowered until the arm wire closes
the circuit with a second contact connected to another PSoC3 GPIO. When the second contact is
touched, the timer will be stopped, and the time will be displayed on the LCD.

This process will be repeated with several ice cubes, both with and without a fan blowing on them,
and the times will be averaged to find out if the ice cubes tended to melt faster, slower, or the
same speed, when the fan is blowing on them.

