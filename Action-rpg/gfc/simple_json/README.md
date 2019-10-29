# simple_json
A free simple json library for C

No Dependencies beyond core C STD libraries.

View the documentation [here](https://engineeroflies.github.io/simple_json/).

This project is meant to add a simple way to parse json files into a lightweight data structure that can be used to store and retreive information parsed from a json file.  The underlying datastructures are not optimized for memory management or fast access.  The intent it to use the SJson type as a way to pull data out of a json file, but it is not supposed to be used when performance matters.

Key Features:
 - Loading and parsing text files containing json formatted string into the SJson data structure
 - Saving a SJson structure back into a correctly formatted json file
 - Echoing out the contents of the json structure to stdout
 - Creation tools to make Objects, Arrays, Strings, NULL, true/false, integers, and floats
 - Getters to pull the data from a SJson structure back out
 - Validation code to check if the data is as expected
 - When errors occur, get the internal message through sj_get_error()

Known limitations:
 - Does not support double (yet) or variations on int (unsigned, short, or long)
 - Does not give object key count
 - Does not give object key list
 - object key lookup is O(n).
