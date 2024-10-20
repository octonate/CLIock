# CLIock
<img src="cliock335.png?raw=true" alt="CLIock" align="right" height="240px">

simple terminal ascii clock\
also can display battery status info

### usage:
run `cliock [options]`

**options:**
by default the clock is displayed and battery info is not
| short | long | description|
|:------|:-----|:-----------|
| -b | --battery | displays battery info |
| -nc | --no-clock | doesn't display the clock |

### configuration:
to configure colors, edit `colors.h` file and change the variables corresponding to each color to whatever color you want\
ex. if you want the hour hand to be bright red, in `colors.h`, the `clockHourHandColor` variable should be set to `BRIGHT_RED`

### installation:
1) download repo from github:\
`git clone https://github.com/octonate/CLIock`

2) cd into CLIock directory:\
`cd CLIock`

3) run make install:\
`sudo make install`

### uninstallation: 
1) in `CLIock` directory run make uninstall:\
`sudo make uninstall`
2) delete `CLIock` directory if you want
