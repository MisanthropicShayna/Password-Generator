# Password Generator (Qt)
This is a general purpose password generator with some unique quirks and features. This was originally part of LSPM, but I decided to rip it out and make it a standalone application. This can be found as a dialog in the current versions of LSPM, but can also be used as a standalone application using this repository.

# Features
There are two generation methods, seed based generation, and occurance based generation. Seed based generation randomly selects N characters from a seed that you input, forming a password. Occurance based selects N amount of characters from each toggled character set. You also have the ability to evenly distribute a set password length across all enabled character sets, so you don't have to specify the amount of characters from each set manually.

The binary comes bundled with the [AMOLED](https://github.com/GTRONICK/QSS/blob/master/AMOLED.qss) QSS theme. If you wish to replace the theme, place the new theme in the same folder as the binary, and make sure it's named `style.qss` as that's what the program looks for. If you don't want any theme, just remove the `style.qss` file that comes bundled, and the OS theme will be used instead.

![](screenshots/mainwindow.png?raw=true)

_Note: I have a OS theme applied on top of the bundled QSS theme, so it may not look exactly identical, however 95% of the elements should look the same._
