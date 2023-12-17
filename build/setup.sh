#!/bin/bash

echo "Creating configuration..."
if [ -d "/var/take-break" ]; then
	echo "directory /var/take-break exists skipping..."
else
	sudo mkdir /var/take-break || echo "failed to create directory /var/take-break" && exit 1
fi

if [ -f "/var/take-break/.config" ]; then
	echo "file .config exists skipping..."
else
	sudo touch "/var/take-break/.config" || echo "failed to create .config file" && exit 1
fi

echo "Writing configuration to .config..."
sudo echo -e "#Seconds\n1200" 1> /var/take-break/.config || (echo "failed to write config to .config file" && exit 1)

echo "Copying the binary file to /usr/bin/take-break..."
sudo cp ./take-break /usr/sbin/take-break || (echo "failed to copy binary to /usr/bin" && exit 1)

echo "Creating a background service..."
sudo touch /etc/systemd/system/take-break.service || (echo "failed to create service file" && exit 1)
sudo echo -e "[Unit]\nDescription=Take Break Service\nAfter=network.target\n\n[Service]\nExecStart=/usr/sbin/take-break\nType=simple\nRestart=always\n\n[Install]\nWantedBy=default.target" > /etc/systemd/system/take-break.service || (echo "failed to write config to service file" && exit 1)

echo "Restarting systemctl..."
sudo systemctl daemon-reload
sudo systemctl enable take-break
sudo systemctl start take-break
