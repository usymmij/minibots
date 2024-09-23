# Client for teleop control

- make sure `Tk` and `python 3` are installed

`python3 teleop-client.py`

## Input scheme
- `WASD` to control
- `Q` to exit

### process
- streams UDP packets to `192.168.4.1`
- packets are 1 byte each with the following format
- only last 4 bits are used
    - first two control left motor
    - next control the right motor


| location (XX) | bits | message             |
|---------------|------|---------------------|
| ---- --XX     | 00   | right motor idle    |
| ---- --XX     | 01   | right motor forward |
| ---- --XX     | 10   | N/A                 |
| ---- --XX     | 11   | right motor reverse |
| ---- XX--     | 00   | left motor idle     |
| ---- XX--     | 01   | left motor forward  |
| ---- XX--     | 10   | N/A                 |
| ---- XX--     | 11   | left motor reverse  |

