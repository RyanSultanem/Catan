# Catan, the Game Logic

This is the heart of the project. It contains all the logic of the Game incorporated as a static library with a simple interface to use it.

## Main Class and Entry Point
In order to abstract all the detail and logic from the user a class with an interface was created with simple functions for using and accessing the Game.

- Class **GameInterface**: 
An interface containing many functions of two types: read functions that will allow the user to know how to display or react (GUI), these functions will not affect the game state or content, and task functions that will alter the game content based on the input, most of these functions retrun a boolean for success/failure of the task.
Through these functions the game can be used and be displayed.

- Class **Game**:
The actual implementation containing all the underlying logic.

## UML class diagram
Under [UML/](UML/) you will find the file Catan.drawio. This contains the main classes and their relations that are used in the implementation. This might help get specific relations without needing to get into the code directly.

In order to access the diagram, you will need to use [Draw.io website](https://app.diagrams.net/) and open the diagram using *Open Existing Diagram*.

