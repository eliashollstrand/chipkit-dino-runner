# Dino Runner Game for chipKIT uC32

![dino-runner-banner]()

A Dino Runner Game developed for the chipKIT uC32 microcontroller using C, $I^2C$ and the MCB32 toolchain.

## Extended Abstract

### Objective and Requirements

The objective for this project was to create a Dino Runner game similar to Google's. That is, a platform game where the player (a running dinosaur) tries to survive for as long as possible by avoiding moving obstacles by jumping or ducking. The obstacles move faster as time goes on and different types of obstacles appear as time goes on.

Out intention was to fulfill the requirements for an **advanced project** through implementation of the features listed below (specifically point 9).

#### Functions we wanted to implemented (points followed by "✓" have been completed)

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

* Playing sound effects through the speaker

### Solution

We have developed our project on the ChipKIT Uno32 board along with the Basic I/O shield. The display on the shield is used to display the game and the buttons are used to jump and duck. We make use of interrupts from the built-in timer (Timer 2) to handle updating the screen as well as increasing the speed of the game.

The I2C protocol is used for reading and writing to the EEPROM. We use the EEPROM to store the leaderboard containing the top 6 high scores, along with the name (3 letters) of the player who achieved the score.

Read and write operations to the EEPROM are performed ....

storing the high score locally so that it is preserved when restarting the game or shutting off the board.

Development was done using the MCB32 toolchain and the main code has been written in the C language together with the pic32mx library. Collaboration and development was facilitated through the use of this GitHub repository and Issues in order to make prioritizations with respect to implementation of new features and bug fixing.

### Verification

Testing has been performed continually throughout the project. During development we systematically tested that our implementations produced satisfying results in addition to testing corner cases such as pressing multiple buttons simultaneously.

### Contributions

This section should explain who did what of the two project members.

### Reflections

The last paragraph should include a few lines of reflections about the project.

<style>h1,h2,h3,h4 { border-bottom: 0; } </style>