# Bees-Live
Beehives with automatic ventilation and temperature regulation system (Hardware)
Demo: https://devpost.com/software/bees-live

![image](https://user-images.githubusercontent.com/60389547/137722714-7cb27990-70a1-4774-b3fa-37934b41f942.png)

## Inspiration
Our group is interested in sustainability. We chose to focus on Bees because are essential for the ecosystem as they contribute with pollinating plants to increase plant growth that other animals depend on as a food source. Honey bees are responsible for pollinating 80% of the plants worldwide. However, unfortunately, due to change in climate, bees are drastically decreasing. Our team will help with ventilating the beehive thereby reducing one of their tasks. Usually, bees flap their wings at the bottom of the beehive to generate air flow in the beehive. Our system will help bees focus on other tasks such as reproduction to increase their population despite drastic changes in temperature due to climate change.

## Source - https://www.uaex.edu/farm-ranch/special-programs/beekeeping/pollinators.aspx

## What it does
Our product tracks and regulates temperature inside beehive and automatically ventilates it based on the current temperature. The application works by allowing the data captured by the AM2301 to be sent to the designed web application. Since bees can only survive between about 40F to 95F, it is important that the temperature does not exceed or go below that threshold. Our system was designed to show case the fluctuation of the temperature to see if it goes above or below threshold. The fan turns on and off based on the temperature readings. If the temperature is higher than 95 it turns on. If its lower than 40 it turns off. In between, it changes according to how close its to 95 or 45.

## How we built it
All the components were connected to a NODMSU(ESP8266) to allow the data to be visually represented on a web. The components connected to the ESP8266 include a L298N motor control gives control over the speed of the fan as the temperature increases or decreases. To measure the temperature, an AM2301 was used to collect the data and send it in real-time to Firebase. Firebase offers security procedures, real-time database, and functions as a service that helped us build a well-integrated product.

## Challenges we ran into
We had members in different time zones, so collaborating was harder. Furthermore, outputting the data on a website was also very challenging as there are different options that be used to implement such a method but we wanted to make sure that method is compatible/objective with our design and is efficient

## Accomplishments that we're proud of
Beehive ventilation and sensing systems do not exist in any beekeeping organization. The idea of introducing technology to beekeeping has never been done before. The novelty of this project along with having it completely work is our accomplishment. And another thing is that we built a finished and well-engineered product that can ship in 24 hours!

## What we learned
All of us learned how to work with either hardware or software and their interconnection for this project. For those who worked in the hardware, working with the ESP8266, L298N motor control, fan, and so on required a lot of work and coordination with lots of learning. For those who worked on the software, working with Firebase for the website and adding sensor data to the online database made the website easier for users and was something that we needed to learn to make a good website.

## What's next for Bees.live
For our application, we want to make sure that we are able to control the fan using code. Doing this will make it easier for users of our application to not worry about their hives all the time. We also want to integrate more features into the website such as notifications for when the temperature is out of the range for bees to survive in the bee hives. Finally, we want to track other factors within the bee hive in the future.

## Future changes
We would love to add a buzzer to alarm the beekeepers if the temperature goes below 40 or above 95

