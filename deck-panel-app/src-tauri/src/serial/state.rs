use std::{error::Error, sync::{Arc, Mutex}};

use serialport::{available_ports, SerialPort, SerialPortInfo, UsbPortInfo};

#[derive(Default)]
pub struct SerialState {
    pub serial_port: Arc<Mutex<Option<Box<dyn SerialPort>>>>,
}


impl SerialState {
    pub fn get_ports() -> Result<Vec<SerialPortInfo>, Box<dyn Error>> {
        let ports = available_ports();

        Ok(ports?)
    }
}
