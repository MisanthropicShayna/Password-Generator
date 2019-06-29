# Advanced Password Generator (Qt)
This is a general purpose password manager with some unique quirks and features. This was originally part of LSPM, but I decided to rip it out and make it a standalone application, and heavily revamped the code too. Now, it comes bundled with the LSPM binary package instead.

# Features
There are two generation methods, seed based generation, and occurance based generation. Seed based generation randomly selects N characters from a seed that you input, forming a password. Occurance based selects N amount of characters from each toggled character set. You also have the ability to evenly distribute a set password length across all enabled character sets, so you don't have to specify the amount of characters from each set manually.
