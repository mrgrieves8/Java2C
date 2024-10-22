# Java-to-C Simulation Project

## Overview

This project demonstrates how Java code is stored in memory and how polymorphism is achieved by recreating Java-like object-oriented structures in C. 
The project provides both a Java file (`java2c.java`) and a C implementation (`test/java2c.c`) that simulates the behavior of Java objects in a low-level language like C.

### Purpose

The project aims to show how:
- **Polymorphism** is simulated in C through the use of function pointers and structures.
- **Memory layout** for objects in Java can be mimicked in C, including method tables and inheritance.

The Java example (`java2c.java`) serves as a blueprint to showcase how a typical object-oriented program behaves in memory, and the corresponding C implementation (`test/java2c.c`) replicates that behavior.

## Class Hierarchy

### In Both Java and C Implementations:
1. **Animal**: The base class for all animals.
    - Contains fields like `num_legs`, `num_masters`, and a unique `ID` for each animal.
    - Implements methods such as `sayHello`, `getNumMasters`, and `toString`.
    - `showCounter` is a static method to display the number of created `Animal` objects.
    - Has an `ID` counter that increments every time a new `Animal` is created.

2. **Dog (extends Animal)**:
    - Inherits from `Animal` and overrides methods like `sayHello`.
    - Has its own fields, such as `num_legs`.

3. **Cat (extends Animal)**:
    - Inherits from `Animal` and allows setting the `colors` of the cat.
    - Has a different number of masters compared to `Animal`.

4. **LegendaryAnimal (extends Cat)**:
    - Further specialization of `Cat`.
    - Overrides methods like `sayHello` and adds specific functionality.
