PCs-connector is an application that allows to use two computers with Linux and Windows with one mouse and one keyboard. Computers communicate with each other by TCP, so both computers have to be in the same network.

Usage:

1. Run pcs_connector.exe binary on the comopuer with Windows and pcs_connector binary on computer with Linux.
2. If you have multiple screens click **Identify screens** button and choose **Border screen** from the drop-down menu.
3. On Linux choose **Server** tab and click **Start** button
4. On Windows choose **Client** tab, put IP of computer with Linux and click **Connect**
5. If both computers are connected properly two rectangles are shown in the application. The red one is the border screen of the computer that runs the application. Blue on is the border screen of the second computer. Change position of the rectangles to adjust border screen position.

LINUX SCREEN

WINDOWS SCREEN



Linux:
In order to build Linux application you need some dependencies:
- Boost 1.70
- Qt 5.14.1

Unit tests:
- Google test

Integration tests:
- Python3
- Pytest

Run unit tests:

cd build
ninja UT


Run integration tests:

ninja integr-tests

if python cannot find binary run:
CONNECTOR_BUILD_DIR=$(pwd) ninja integr-tests
