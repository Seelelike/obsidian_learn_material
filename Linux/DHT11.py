import wiringop
import time

class DHT11:
    def __init__(self, pin):
        self.pin = pin
        wiringop.wiringPiSetup()
        
    def read(self):
        """读取传感器数据并返回(湿度%, 温度℃)"""
        try:
            bits = self._read_raw_bits()
            return self._parse_data(bits)
        except Exception as e:
            print(f"传感器读取失败: {str(e)}")
            return None, None
    
    def _read_raw_bits(self):
        """读取40位原始数据位"""
        # 发送开始信号
        wiringop.pinMode(self.pin, wiringop.OUTPUT)
        wiringop.digitalWrite(self.pin, wiringop.LOW)
        self._delay_us(18000)  # 18ms低电平
        wiringop.digitalWrite(self.pin, wiringop.HIGH)
        self._delay_us(20)     # 主机拉高30µs
        
        # 等待传感器响应
        wiringop.pinMode(self.pin, wiringop.INPUT)
        self._wait_for_level(wiringop.LOW, 100)   # 80µs低电平
        self._wait_for_level(wiringop.HIGH, 100)  # 80µs高电平
        
        # 读取40位数据
        bits = []
        for _ in range(40):
            self._wait_for_level(wiringop.LOW, 60)  # 50µs起始低电平
            pulse_width = self._measure_pulse(wiringop.HIGH)  # 测量高电平
            bits.append(1 if pulse_width > 50 else 0)  # 阈值50µs
        
        return bits
    
    def _parse_data(self, bits):
        """解析40位数据并验证校验和"""
        if len(bits) != 40:
            raise ValueError(f"数据长度错误，预期40位，实际{len(bits)}位")
            
        bytes_data = [self._bits_to_int(bits[i:i+8]) for i in range(0, 40, 8)]
        
        # 验证小数部分为0
        #if bytes_data[1] != 0 or bytes_data[3] != 0:
        #    raise ValueError("DHT11小数位应为0，数据异常")
        
        # 校验和验证
        checksum = (bytes_data[0] + bytes_data[1] + bytes_data[2] +bytes_data[3]) & 0xFF
        if checksum != bytes_data[4]:
            raise ValueError(f"校验和错误: 预期{bytes_data[4]} 实际{checksum}")
        
        return bytes_data[0], bytes_data[2]  # 湿度, 温度
    
    def _wait_for_level(self, level, timeout_us):
        """等待引脚电平变化"""
        start = time.perf_counter()
        while wiringop.digitalRead(self.pin) != level:
            if (time.perf_counter() - start) * 1e6 > timeout_us:
                raise TimeoutError("电平等待超时")
    
    def _measure_pulse(self, state):
        """测量脉冲宽度（µs）"""
        start = time.perf_counter()
        while wiringop.digitalRead(self.pin) == state:
            pass
        return (time.perf_counter() - start) * 1e6
    
    def _delay_us(self, microseconds):
        """微秒级延时"""
        start = time.perf_counter()
        while (time.perf_counter() - start) * 1e6 < microseconds:
            pass
    
    def _bits_to_int(self, bits):
        """二进制位列表转整数"""
        return int(''.join(map(str, bits)), 2)

# 使用示例

sensor = DHT11(pin=16)  # 使用物理引脚16
    
while True:
    humidity, temperature = sensor.read()
    if humidity is not None and temperature is not None:
        print(f"湿度: {humidity}%  温度: {temperature}°C")
    else:
        print("等待重试...")
    time.sleep(2)  # DHT11需要至少1秒间隔