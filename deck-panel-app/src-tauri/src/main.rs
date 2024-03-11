// Prevents additional console window on Windows in release, DO NOT REMOVE!!
#![cfg_attr(not(debug_assertions), windows_subsystem = "windows")]
pub mod serial;
use core::panic;
use std::{sync::{Arc, Mutex}, time::Duration};

use serial::state::SerialState;
use serialport::SerialPortType;
use tauri::Manager;

fn main() {
    tauri::Builder::default()
        .setup(|app| {
            let mut serial_state = SerialState::default();
            let ports = SerialState::get_ports()?;

            for port in ports {
                println!("{port:?}");

                let SerialPortType::UsbPort(data) = port.port_type else {
                    continue;
                };

                if !(data.vid == 9025 && data.pid == 66) {
                    continue;
                }

                let serial_port = serialport::new(port.port_name, 9600).open()?;

                serial_state.serial_port = Arc::new(Mutex::new(Some(serial_port)));
            }

            {
                let serial_port = serial_state.serial_port.lock().unwrap();

                if serial_port.is_none() {
                    panic!("couldn't find serial port");
                }

            }

            let port_clone = Arc::clone(&serial_state.serial_port);
            std::thread::spawn(move || {
                loop {
                    let mut serial_port = port_clone.lock().unwrap();


                    let mut buf = [0; 8];

                    if serial_port.is_none() {
                        return;
                    }

                    let serial_port = serial_port.as_mut().unwrap();
                    if let Ok(..) = serial_port.read(&mut buf) {
                        println!("{buf:?}");
                    }
                }
            });

            app.manage(serial_state);

            Ok(())
        })
        .invoke_handler(tauri::generate_handler![])
        .run(tauri::generate_context!())
        .expect("error while running tauri application");
}
