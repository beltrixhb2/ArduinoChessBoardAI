# ArduinoChessBoardAI
Here's the translation into English:

We are pleased to introduce our chess project with integrated artificial intelligence. In this work, we have combined the essence of chess with the capabilities of artificial intelligence, all represented through a matrix of LEDs.

Our team, consisting of Lucia, Beltran, Ángel, and Javier, has dedicated itself to designing and developing a chessboard. This process has been a challenge that has brought several problems for which we have found solutions, as well as difficulties from which we have learned and overcome to achieve our goal.

To facilitate interaction with the game, we have created a display that, along with some buttons, allows for intuitive and fluid handling of positions. Additionally, we have implemented a 16x16 LED matrix that reflects the AI’s source and destination squares, providing an immersive and dynamic visual experience.

As we have mentioned before, with this project, we seek to provide a solution for those who love chess but, due to availability or other reasons, cannot find an opponent to play with. With this chess set, they can enjoy a good game whenever they want.

## MODEL

Starting with the most visual aspect of our project, we will discuss the model:

We can see that our chess set first consists of a box that houses the circuits, display, Arduino, and on which our board sits. This box, essential for the operation and aesthetics of the project, was meticulously crafted using plywood with side measurements of 19.8 cm x 10 cm. For its construction, we have used simple but effective tools: a cutter to set the dimensions accurately, "No More Nails" adhesive to ensure a solid and durable joint, and small wooden blocks to provide stability and robustness to the structure.

Once the box was constructed, with the help of small wooden stops, we achieved a solid base on which to introduce all the necessary electronic components, including the circuit, display, buttons, and Arduino. Strategically, we made a hole in the front wall of the box to allow the display and buttons to come out, thus providing comfortable and practical access during the game.

As a final touch and to provide an elegant and functional appearance to the chess set, we chose to use methacrylate divided into squares as a lid, on which we placed wooden edges where we marked the corresponding letters and numbers. This design not only adds an aesthetic element to the assembly but also facilitates the identification of squares during the game.

In summary, through this design and manufacturing process, we have managed to create a chess set that not only meets functional and technical requirements but also stands out for its comfort, aesthetics, and practicality in each game.

## BUTTONS AND DISPLAY

Next, we will detail the code used in our project as briefly as possible. To facilitate the explanation, we will divide the code into three parts, taking into account the hardware they handle, plus the setup; however, there will be functions that employ elements corresponding to more than one part:

1.     Setup: First, it is necessary to include libraries and define the pins and global variables that we will use in our program. As can be seen at a glance, there are a large number of defined variables and data, most of which belong to the AI used as the opponent in our chess game. We will proceed to explain only the most relevant elements.

Libraries: We use LiquidCrystal for handling the display and Adafruit_NeoPixel for managing the LED matrix.

Variables:

AI: The variable we have introduced to manage the AI through our hardware is inputString, a String where the movement to be made will be stored and which we will also use as a control tool to know if more data needs to be introduced to complete the movement, or if, on the contrary, it is already complete. We will also use lastM and lastH, representing the last movement made by the AI and the human, respectively.

LCD: Object to manage and initialize the display, with 16 columns by 2 rows.

Matrix: Object to manage and initialize the 16x16 LED matrix.

Buttons: Here we have created 3 global integer variables, as well as one of type char. The integer variables consist of position, to keep track of the last filled position of inputString and which also indicates the cursor position on the display; digit, to store in inputString the digits of the movement to be made; and counter, a variable which will increase as buttons are pressed and will represent the digit or character to be entered. The char type variable is letter, analogous to the digit variable.

Pins: We reserve digital pins 2, 4, and 7 for the buttons, digital pins 8, 9, 10, 11, 12, and 13 for the display, and digital pin 0 for the LED matrix.

2.     Buttons and display: In this section, we find a main function, read_movement(). This function detects the pressing of buttons and performs the pertinent action to the pressed button. It is also found in the main

 program (loop()) and is executed in a loop while inputString is not complete. Each button, which we will call button1, button4, and buttonNext, has one or more possible actions, which will be visible on the display, and it will be decided which to execute depending on the state of inputString (remember that we control its state through the position variable).

Initially, button1 has 3 possible actions: if the position variable is even, it represents that a letter is desired to be introduced to make the movement, so each press of this button will iterate from 'a' to 'h' one by one. After reaching 'h' and continuing to iterate, it will return to the beginning 'a', which will be controlled by the function topeDigito().

Similarly, if the position is odd, the same procedure will be carried out, but with the numbers from 1 to 8. Lastly, the special case where the position is equal to 4, which represents that all the necessary data has been introduced to make the movement. If you wish to confirm the introduced movement (in case of having made a mistake) this button will be pressed and the movement will be accepted, waiting to check if it is valid. In this special case, inputString is completed, thus fulfilling the function that ends the loop where read_movement() was found.

Analogously, button4 has 3 possible analogous actions to button1, with the difference that it iterates in 4s instead of 1s, controlled by the function topeDigito4().

For the case where the position is equal to 4, if you wish to reject the movement (since the movement has been introduced with some error) this button will be pressed, which will cause inputString to be erased and position to return to 0, through the function anulado(), and therefore the loop where read_movement() is found will not be finished.

Lastly, buttonNext has 1 function: while the position is not 4, meaning that not all the necessary data has yet been introduced, each time this button is pressed it will serve as confirmation that the element of the movement we were currently introducing, whether digit or letter, is correct, it will be stored in inputString, and we will proceed to introduce the next element of the movement. To do this, through topePosición() we increase the value of the position by 1. Also, as before, if the position is even it means that a letter is being introduced and if it is odd, a figure.

To conclude, there are three functions that employ the logic of the buttons and another that also mixes such logic with that of the LED matrix and the AI; however, they are not activated by pressing these buttons. They are invalid(), activated when a movement confirmed by the human is not correct; win(), when the human wins the game; and lose(), when the human loses the game. All three conditions show relevant information on the display.

However, we have not indicated at any time a function that is solely responsible for the logic of the display. This is because there is none, all the actions performed by the display, which are to show on-screen information related to the movement, are embedded in the functions mentioned above. The management of the cursor position on the display is carried out through the position variable, and once the cursor position is introduced, the desired message is printed using the predefined LCD.write() function.

## INTEGRATED ARTIFICIAL INTELLIGENCE

For the project, we have implemented a chess module that incorporates artificial intelligence (AI) to face the user in chess games. This central module of our application is based on a source code obtained from the internet, which has been adapted and modified to meet our specific requirements.

The artificial intelligence that we have adopted and modified is known as "Arduino UNO MicroMax Chess," originally developed by Diego Cueva. This version is based on "Micro-Max," version 4.8, a creation by H.G. Muller. "Micro-Max" is the most compact chess program and artificial intelligence available, consisting of less than 2000 characters and just 133 lines of code. Despite its small size, it is capable of fully executing the game of chess, respecting all the rules set by the International Chess Federation (FIDE).

The choice of "Micro-Max" for our project was not random, as its extremely compact size makes it ideal for the limited memory of our Arduino board. However, this size also presents challenges, mainly due to the abstraction and condensation of the code. For example, the name of all functions and variables is limited to a single character, which complicates the task of understanding and modifying the code without detailed documentation and a deep understanding of its internal mechanics.

The main adaptations that we have had to implement in the code include the integration of movement capture through physical buttons, the implementation of animations using LEDs for more visual interaction, as well as the representation of information and movements on a connected display to our

 central unit. We have had to replace the way the module had to receive movement with a call to our own function, which would collect the input through the buttons and display the necessary information on the display screen. After each movement made by the artificial intelligence, we have added the call to another function that would print on the display screen and in the LED matrix the movement made. Additionally, we have located the points in the code where it was identified that an invalid movement had been made, a game had been won, or a game had been lost, and inserted the call to the functions that would show the relevant animation for each of them.

## LED MATRIX

For the management of the LED board, we have used different functions.
To start and as an introduction, we start from a 16 x 16 LED board, so each square is equivalent to 4 LEDs (we planned it this way to make it more visual, since using only one LED per square would make the board too small).

In principle, the LED board can be divided into two major processes.

The lighting and extinguishing of the origin and destination square: the movements of the AI are transferred by lighting at the same time both the square where the piece wants to move and the square to which it wants to move.
The squares will remain lit until the next movement.

LIGHTCASILLA(char, int, int, int, int) is the function we have implemented to carry out this action.

Given that there are few lines of code, we can include the same function to better develop its explanation and understanding.

Various animations. These animations occur in specific situations and we have implemented them so that the message we wanted to convey was more visual.

WINANIMATION(): once the game is over, and the result has been "winner," an animation will appear in which the letters WIN will appear sequentially in green.

LOSEANIMATION(): once the game is over, and the result has been "loser," an animation will appear in which the letters LOSE will appear sequentially in red.

INVALIDANIMATION(): this animation is implemented when a move is not valid, that is, it is not allowed.
This animation is very simple, as the only thing it shows is a red X across the board.

NOTE: for all the functions mentioned above for the different animations, we have implemented an auxiliary function APAGARTODO(), which we call continuously either when showing the letters of winning or losing; or when the next move starts after the movement of the AI.
