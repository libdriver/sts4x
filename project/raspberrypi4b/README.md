### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

IIC Pin: SCL/SDA GPIO3/GPIO2.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```

#### 2.2 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.3 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(sts4x REQUIRED)
```


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
./sts4x -i

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
./sts4x -p

sts4x: SCL connected to GPIO3(BCM).
sts4x: SDA connected to GPIO2(BCM).
```

```shell
./sts4x -t read --addr=0 --times=3

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
sts4x: temperature is 24.34C.
sts4x: temperature is 24.33C.
sts4x: temperature is 24.31C.
sts4x: medium precision.
sts4x: temperature is 24.31C.
sts4x: temperature is 24.27C.
sts4x: temperature is 24.27C.
sts4x: lowest precision.
sts4x: temperature is 24.21C.
sts4x: temperature is 24.21C.
sts4x: temperature is 24.21C.
sts4x: soft reset.
sts4x: finish read test.
```

```shell
./sts4x -e read --addr=0 --times=3

sts4x: 1/3.
sts4x: temperature is 28.51C.
sts4x: 2/3.
sts4x: temperature is 28.75C.
sts4x: 3/3.
sts4x: temperature is 28.95C.
```

```shell
./sts4x -e number --addr=0

sts4x: serial number is 0x11 0x25 0xB3 0xF2.
```

```shell
./sts4x -h

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

