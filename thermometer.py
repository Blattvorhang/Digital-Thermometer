import serial
import re
import numpy as np
import matplotlib.pyplot as plt
#from matplotlib.animation import FuncAnimation

def volt2temp(volt): #mV
    res_x = 11 / (3992 / volt - 1)
    temperature = 1 / (1 / 298.15 - np.log(10 / res_x) / 3950) - 273.15
    return temperature

def instantiate(func):
    return func()
#instantiate = lambda func : func()

@instantiate
def plottemp():
    '''
    temperature chart
    '''
    
    x = y = np.empty(0)
    i = 0
    delay = 0.2   #两次数据接受的时间间隔
    amount = 100  #需要统计的数据量
    arange_amount = np.arange(0, delay * amount, delay)
    ones_amount = np.ones(amount)
    
    def inner(temp):
        nonlocal x, y, i
        
        # data entry
        x = np.append(x, i)
        y = np.append(y, temp)
        i += delay
        if i > delay * amount:
            x = np.delete(x, 0)
            y = np.delete(y, 0)
        mean = np.mean(y)
        var = np.var(y)
        
        # data plot
        plt.clf()
        plt.title('mean:{:.2f}    variance:{:.4f}'.format(mean, var))
        plt.xlabel('time/s')
        # plt.ylabel('temperature/℃')
        plt.plot(x, y, label='temperature/℃')
        if i < delay * amount:
            plt.plot(arange_amount, ones_amount * mean, 'r--', linewidth=0.8, label='mean/℃')
            plt.xlim(0, delay * amount - delay)
        else:
            plt.plot(x, ones_amount * mean, 'r--', linewidth=0.8, label='mean/℃')
            plt.xlim(i - delay * amount, i - delay)
        plt.legend()
        plt.pause(0.01)
        plt.ioff()
        
    return inner

if __name__ == '__main__':
    ser = serial.Serial('COM7', 115200, timeout=0.5)
    while True:
        line = ser.readline().decode('gbk')
        # print(line)
        if len(line) > 0:
            # volt = int(re.sub(r'\D', '', line))
            # plottemp(volt2temp(volt))
            temp = float(re.sub(r'[^0-9.]', '', line))
            plottemp(temp)
