# Dino Runner Game for chipKIT uC32

![dino-runner-banner](https://github.com/eliashollstrand/chipkit-dino-runner/tree/main/resources)

A Dino Runner Game developed for the ChipKIT uC32 microcontroller using C, $I^2C$ and the MCB32 toolchain.

Elias Hollstrand  & Mattias Kvist

## Extended Abstract

### Objective and Requirements

The objective for this project was to create a Dino Runner game similar to Google's on a ChipKIT uC32 microcontroller. That is, creating a platform game where the player (a running dinosaur) tries to survive for as long as possible by avoiding moving obstacles by jumping and ducking. The obstacles move faster as time goes on and different types of obstacles appear as time goes on.

Our intention was to fulfill the requirements for an **advanced project** through implementation of the features listed below (specifically point 9).

#### Functions we wanted to implement (points followed by "✓" have been implemented)

1. Drawing ground ✓
2. Drawing the player on the screen (on the ground) ✓
3. Drawing obstacles on the screen (cacti, birds) ✓
4. Handling user input from the buttons ✓
5. Game physics and animation (jumping, ducking, moving objects sideways) ✓
6. Increasing the speed of obstacles with time ✓
7. Birds appear after certain amount of time has passed
8. Calculating and drawing current score on the screen on each frame ✓
9. Saving high score using I2C EEPROM ✓
10. Game menu (display high score, start game, credits) ✓

#### Optional features we thought of implementing if time would allow

* Playing sound effects through the built-in speaker

### Solution

We have developed our project on the ChipKIT Uno32 board along with the Basic I/O shield. The display on the shield is used to display the game and the buttons are used to jump and duck. We make use of interrupts from the built-in timer (Timer 2) to handle updating the screen as well as increasing the speed of the game, and dealing with user input.

The I2C protocol is used for reading and writing to the EEPROM. We use the EEPROM to store the leaderboard containing the top 6 high scores, along with the name (3 letters) of the player who achieved the score. The functions for read and write operations to the EEPROM
were derived from the [Microchip 24LC256 datasheet](https://ww1.microchip.com/downloads/aemDocuments/documents/MPD/ProductDocuments/DataSheets/24AA256-24LC256-24FC256-256K-I2C-Serial-EEPROM-DS20001203.pdf). They were implemented in the 'highscore.c' file, with the help of the functions in the 'i2c-func.c' file. To know if the bus is not busy, acknowledge polling is used. The functions for reading make use of sequential reads, and writing uses page writing.

Animations are implemented by checking the number of frames that one of the images for a character has been printed for and switching to another image when a certain number of frames have passed, making for an animated effect.

The leaderboard consist of two arrays, one for the scores and one for the names, which are read from EEPROM on startup with the function `read_leaderboard`. When a new high score is achieved, the player is prompted to enter their name using the buttons. The name is then written to EEPROM along with the score. The leaderboard is then read from EEPROM again and displayed on the screen. Reading and writing to EEPROM is done using the functions `read_multiple_scores`, `read_initials`, `write_multiple_scores` and `write_initials`. The functions for reading make use of sequential reads, and writing uses page writing.
The highest score is in `leaderboard_scores[0]`, and is also displayed on the screen during gameplay.

Development was done using the MCB32 toolchain and the main code has been written in the C language together with the pic32mx library. Collaboration and development was facilitated through the use of the VSCode extension Live Share and this GitHub repository along with Issues in order to make prioritizations with respect to implementation of new features and bug fixing.

### Verification

Testing has been performed continually throughout the project. During development we systematically tested that our implementations produced satisfying results in addition to testing corner cases such as pressing multiple buttons simultaneously and performing actions in different orders.

### Contributions

Much of the development was done using the VSCode extension [Live Share](https://marketplace.visualstudio.com/items?itemName=MS-vsliveshare.vsliveshare) which allowed us to work on the same code simultaneously. This was especially useful when working on the same file, as we could see each other's cursors and selections in real time. This method was used for the majority of the most time consuming implementations of advanced functionalities such as $I^2C$ implementation, setup and implementation of the built-in timer as well as implementation of timer interrupts and change notice interrupts (which we ended up not using). Less advanced parts which we worked on together include drawing pixel art characters, text and numbers.

Elias worked alone on the functionality of animation.

Mattias worked alone on the functionality of game physics, switching between different game states and the initial implementation of the leaderboard with multi read/write operations.

### Reflections

#### What went well?

We are very happy with the result of the project. We managed to implement all of the features we wanted to implement, and we are especially happy with the leaderboard functionality. We are also happy with the animations, the physics and the general look and feel of the game.

#### What could have been better?

We stumbled upon lots of bugs that were hard to find and fix. This was especially true for the everything that had to do with $I^2C$ and the leaderboard functionality, which was the last thing we implemented. We also had some issues with the change notification interrupt, which we ended up not using. Too much time wast spent debugging and when we finally resolved one bug, another one appeared, which was very frustrating.

We had planned to do all the development together, but due to Elias being sick for a week, Mattias had to do some of the development alone.

#### What have we learned?

We have learned a lot about the $I^2C$ protocol and how to use it to read and write to EEPROM. We have gained a better understanding of how to use interrupts to handle user input and update the screen. We also improved our debugging skills, as we had to do a lot of it.
