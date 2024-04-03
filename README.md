# custom-taser
Hardware and software needed for my custom [TENS unit](https://en.wikipedia.org/wiki/Transcutaneous_electrical_nerve_stimulation). Made with the purpose of making it easier and faster to integrate a TENS unit with other hardware / software for custom projects.

### Hardware
The hardware essentially boils down to 4 H-bridges, and a boost converter. The H-bridges are driven straight from the pins of an arduino nano, and the boost converter is driven by a dedicated arduino nano. 2 separate boards are used because the timing needed for a boost converter is relatively sensitive, and if the H-bridges are activating, the boost converter timing is thrown off, resulting in voltages that are not consistent. There is a voltage divider on the output of the boost converter that gives feedback to the arduino board. The analogRead value should not be near either of the value extremities for the arduino board, as the readings are not perfectly linear, and tend to snap to the max and min read values.

### Software
The software creates an AC square wave on the channel that is asked of it. Frequency should be somewhere from [30 - 50Hz](https://enrichedhealthcare.com/library/resources/tens/#:~:text=80%20to%20120Hz%2Dacute%20pain%0A%0A35%2D50Hz%2Dmuscle%20stimulation%0A%0A2%20to%2010Hz%20%E2%80%93%20chronic%20pain) in order to stimulate the muscle properly. The boost converter uses extremely simple controls as of now, but I plan to give it PID control soon.

### Usage
I used this project to shock users in [this](https://youtu.be/dzjZxOKlJjM) video of my friends playing mario kart. I have plans to use it more in the coming years.
