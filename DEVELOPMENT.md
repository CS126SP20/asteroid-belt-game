# Development

---
 **4/18/20:** 
- Created Asteroid and Ship class
- Added user input functionality for the ship

 **4/19/20:** 
- Added animation to the asteroids in the update() method
- Created a method to detect collisions between the asteroids and ship

 **4/21/20:** 
 - Implemented multiple libraries: sqlite-modern-cpp, sqlite3, and gd-graphics
 - Created text printing methods
 - Created database to store player names and scores
 
 **4/23/20:** 
 - Added function to the database to retrieve the highest score
 - Updated asteroid animation to make the game continuous

 **4/24/20:** 
 - Implemented gflags library to parse the command line for player name
 - Created title screen that is displayed first when the program is run
 
 **4/25/20:** 
 - Added varying difficulties to the game: easy, medium, hard
 
 **4/28/20:** 
 - Loaded a starry image into the background
 - Added a game over screen that displays the highest score from the database
 - Added user functionality so that they can choose which difficulty they are playing on
 
  **4/29/20:** 
  - Added clipart to asteroids and the spaceship

  **4/30/20:** 
  - Created a more accurate collision detecting method using rectangles
  
  **5/1/20:** 
   - Updated the positions of the clipart on the home screen 
   - Created a new rules screen that displays the rules of the game
     
  **5/2/20:** 
   - Updated the positioning of the clipart on the game over screen
   - Did minor code cleaning (deleting whitespaces and updated certain variable names)
   
  **5/3/20:** 
  - Created the laser class which includes update, draw, and get location methods
  - Implemented these methods into my_app.cc to display laser animation
  - Implemented laser collision detecting method
   
   **5/4/20:** 
   - Created a DisplayText method that prints text in one line
   - Finished updating the game over screen by adding game stats
   - Added audio to the program that plays when the game starts
   
   **5/5/20:** 
   - Cleaned up all the code in my_app.cc (Removed more than 100 lines of repeated code, updated all variable and method names to meet Google C++ standard)
   - Replaced boolean game state with the enum class GameState
   - Created utilities class to modularize code and refactored helper methods
   
   **5/6/20:** 
   - Wrote tests for the helper methods in the Utilities class
   - Updated the README.md
   - Added PROPOSAL.md




