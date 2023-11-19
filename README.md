# Spell-Checker

This C program implements a spell-checking application that reads words, meanings, and grammars from a CSV file, builds a balanced binary search tree (BST) with case-insensitive insertion, allows the user to check the spelling of a sentence, and updates the CSV file with any new words added during the spell-checking process.

Data Structures:

    Binary Search Tree (BST):
        struct TreeNode with members word, meaning, grammar, left, and right.

    Arrays of Strings:
        Arrays (words, meanings, grammars) to store words, meanings, and grammars read from the CSV file.

Functions:

    Tree Manipulation Functions:
        createNode, insertCaseInsensitive, createBalancedBST, findSmallestAndLargest, freeTree.

    String Manipulation Functions:
        toLowercase, removeHyphens, removePunctuation.

    File I/O Functions:
        readCsvFile, writeCsvFile.

    User Interaction Functions:
        checkSpellingOfSentence, printSmallestAndLargest.

    Memory Management Function:
        freeArray.


Commands:
      gcc -c spell_checker.c
      gcc -o spell_checker spell_checker.o
      spell_checker
