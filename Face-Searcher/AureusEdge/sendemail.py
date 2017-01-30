import smtplib
import sys

sender = sys.argv[1]
password = sys.argv[2]
receiver = sys.argv[3]
messageFile = sys.argv[4]

server = smtplib.SMTP('smtp.gmail.com', 587)
server.starttls()
server.login(sender, password)

message = open(messageFile, 'r').read()
server.sendmail(sender, receiver, message)
server.quit()
