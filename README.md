# DataParser
通信中数据的打包和拆包

# DataFrame
Header | Handle(DataType) | Length | payload | Sum
-------| -----------------| -------| --------| ----
0xAA   | 

### how to use
```
App_uart_Send(enum UartHandle handle, uint8_t* data, uint16_t length);

App_uart_Receive(const uint8_t* packet, uint16_t length);

```

##### 需要自己实现的
### 1.
void LinkLayer_Send(packet, length)
{
  ···
}

### 2.在底层接收数据的接口，将数据，数据长度传入函数
```
void App_uart_Receive(const uint8_t* packet, uint16_t length)
```

### 3. 自定义数据类型
enum UartHandle{
 ...
 MaxHandle
}
