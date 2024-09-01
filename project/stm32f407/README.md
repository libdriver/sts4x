### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

IIC Pin: SCL/SDA PB8/PB9.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. STS4X

#### 3.1 Command Instruction

1. Show sts4x chip and driver information.

   ```shell
   sts4x (-i | --information)
   ```

2. Show sts4x help.

   ```shell
   sts4x (-h | --help)
   ```

3. Show sts4x pin connections of the current board.

   ```shell
   sts4x (-p | --port)
   ```

4. Run sts4x read test, num is test times.

   ```shell
   sts4x (-t read | --test=read) [--addr=<0 | 1 | 2>] [--times=<num>]
   ```

5. Run sts4x read function, num is read times.

   ```shell
   sts4x (-e read | --example=read) [--addr=<0 | 1 | 2>] [--times=<num>]
   ```

7. Run sts4x number function.

   ```shell
   sts4x (-e number | --example=number) [--addr=<0 | 1 | 2>]
   ```

#### 3.2 Command Example

```shell
sts4x -i

sts4x: chip is Sensirion STS4X.
sts4x: manufacturer is Sensirion.
sts4x: interface is IIC.
sts4x: driver version is 1.0.
sts4x: min supply voltage is 1.1V.
sts4x: max supply voltage is 3.6V.
sts4x: max current is 0.50mA.
sts4x: max temperature is 125.0C.
sts4x: min temperature is -40.0C.
```

```shell
sts4x -p

sts4x: SCL connected to GPIOB PIN8.
sts4x: SDA connected to GPIOB PIN9.
```

```shell
sts4x -t read --addr=0 --times=3

sts4x: chip is Sensirion STS4X.
sts4x: manufacturer is Sensirion.
sts4x: interface is IIC.
sts4x: driver version is 1.0.
sts4x: min supply voltage is 1.1V.
sts4x: max supply voltage is 3.6V.
sts4x: max current is 0.50mA.
sts4x: max temperature is 125.0C.
sts4x: min temperature is -40.0C.
sts4x: start read test.
sts4x: serial number is 0x11 0x25 0xB3 0xF2.
sts4x: high precision.
sts4x: temperature is 27.13C.
sts4x: temperature is 27.11C.
sts4x: temperature is 27.11C.
sts4x: medium precision.
sts4x: temperature is 27.03C.
sts4x: temperature is 27.03C.
sts4x: temperature is 27.03C.
sts4x: lowest precision.
sts4x: temperature is 26.95C.
sts4x: temperature is 27.03C.
sts4x: temperature is 26.95C.
sts4x: soft reset.
sts4x: finish read test.
```

```shell
sts4x -e read --addr=0 --times=3

sts4x: 1/3.
sts4x: temperature is 26.69C.
sts4x: 2/3.
sts4x: temperature is 26.67C.
sts4x: 3/3.
sts4x: temperature is 26.66C.
```

```shell
sts4x -e number --addr=0

sts4x: serial number is 0x11 0x25 0xB3 0xF2.
```

```shell
sts4x -h

Usage:
  sts4x (-i | --information)
  sts4x (-h | --help)
  sts4x (-p | --port)
  sts4x (-t read | --test=read) [--addr=<0 | 1 | 2>] [--times=<num>]
  sts4x (-e read | --example=read) [--addr=<0 | 1 | 2>] [--times=<num>]
  sts4x (-e number | --example=number) [--addr=<0 | 1 | 2>]

Options:
      --addr=<0 | 1 | 2>
                        Set the addr.([default: 0])
  -e <read | number>, --example=<read | number>
                        Run the driver example.
  -h, --help            Show the help.
  -i, --information     Show the chip information.
  -p, --port            Display the pin connections of the current board.
  -t <read>, --test=<read>
                        Run the driver test.
      --times=<num>     Set the running times.([default: 3])
```

