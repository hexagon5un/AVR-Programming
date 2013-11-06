import math
import random

CENTER = 128
NOISE = 3

def badIntegerEWMA(data, initialValue = CENTER):
    averages = []
    average = initialValue;
    for x in data:
        average = (int(x) + 7*average) / 8
        averages.append(average)
    return(averages)

def fixedIntegerEWMA(data, initialValue = CENTER):
    averages = []
    average = initialValue;
    for x in data:
        average = (int(x) + 7*average + 4) / 8
        averages.append(average)
    return(averages)


def floatEWMA(data, initialValue = CENTER):
    averages = []
    average = initialValue;
    for x in data:
        average = (x + 7*average) / 8 
        averages.append(average)
    return(averages)
    
def runComparo(length, initialValue):
    actualValue = [random.uniform(CENTER-NOISE, CENTER+NOISE) for x in range(length)]
    return(
        zip(badIntegerEWMA(actualValue, initialValue), 
            fixedIntegerEWMA(actualValue, initialValue), 
            floatEWMA(actualValue, initialValue))
    )

def runExperiment():
    for i in range(1000):
        initialValue = int(round(random.uniform(CENTER-5*NOISE, CENTER+5*NOISE)))
        values = runComparo(100, initialValue)[-1]
        print initialValue, values
