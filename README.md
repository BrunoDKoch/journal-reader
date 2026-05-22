# Journal Reader

## Brief explanation

Journal Reader is a very simple GUI application for reader `journalctl` records, written in C++ with Qt6.

It's just something I made for a few friends who are getting into Linux and might not be very used to reading logs on the terminal. It's not meant to be anything big or serious, it's more of a tiny hobby project.

Heck, the icon is literally just a combination of two SVG icons I found on [icones](https://icones.js.org). Specifically, `streamline-ultimate-color:common-file-text` and `streamline-color:magnifying-glass`. I just slapped them together because I thought it looked kinda okay. I'm no artist and I don't want to use image generators for something like this (or at all, really).

## How to use

Please be mindful that this is an alpha build. Issues are expected and new features will be added.

Upon opening the program, you'll be greeted by 3 elements:

1. A top bar with two buttons for date filtering ("Since" and "Until"), a text bar for regular expression searches, a "Submit" button for performing those searches, and a dropdown with priority levels ("NO PRIORITY", "EMERGENCY", "ALERT", "CRITICAL", "ERROR", "WARNING", "NOTICE", "INFO" and "DEBUG").

2. A table with the records of your search (by default, it just grabs the records from your current boot with no filters applied), sorted from newest to oldest.

3. A numerical input for the boot to be searched. 0 is the current one, 1 is the previous one, 2 the one before that, and so on.

To filter by date and time, press either the "Since" or "Until" button. This will open a simple date-time widget, where you can input the desired value. Please note that you cannot use both filters at once currently (this is planned for the future).

You also cannot currently sort the items on the display table. This might change in the future.

Double-clicking on a record will open a detailed view with all the fields saved by `journalctl`.

You can also export logs by going to `File -> Export Logs...` or by pressing Ctrl + S. This will export all logs that are currently loaded.

You can copy highlighted logs by going to `Edit -> Copy Selected` or by pressing Ctrl + C.

Finally, you can refresh the page by going to `Edit -> Refresh` or by pressing F5.

## Compatibility

Jornal Reader is only guaranteed to be compatible with Arch Linux and derivatives. While I can look into packaging this for Fedora and Debian eventually, I can't make any guarantees.

As stated before, this is a hobby project, and both myself and the friends I made this for use either Arch Linux or a derivative like CachyOS.

Plus, while Fedora and its derivatives tend to be relatively up-to-date with their packages, the Debian family of distros is more focused on stability. I
