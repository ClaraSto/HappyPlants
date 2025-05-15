import smtplib
from email.message import EmailMessage
import json

#Set the sender email and password and recipient emaiç
with open("config.json", "r") as f:
    config = json.load(f)
from_email_addr = json.loads(config["from_email_addr"])
from_email_pass = json.loads(config["from_email_pass"])
to_email_addr = json.loads(config["to_email_addr"])

# Create a message object
msg = EmailMessage()

# Set the email body
body ="Hi, deine Pflanze ist "
msg.set_content(body)

# Set sender and recipient
msg['From'] = from_email_addr
msg['To'] = to_email_addr

# Set your email subject
msg['Subject'] = 'TEST EMAIL'

# Connecting to server and sending email
# Edit the following line with your provider's SMTP server details
server = smtplib.SMTP('smtp.gmail.com', 587)

# Comment out the next line if your email provider doesn't use TLS
server.starttls()
# Login to the SMTP server
server.login(from_email_addr, from_email_pass)

# Send the message
server.send_message(msg)

print('Email sent')

#Disconnect from the Server
server.quit()
