#include "EmailConfig.h"

void SendEmail(EmailConfig *email)
{
	char command[1024];
	char piece[1024];
	strcpy(command, "C:/Allevate/Face-Searcher/AureusEdge/sendemail.py");

	std::ofstream messageFile;
	messageFile.open("C:/Allevate/Face-Searcher/AureusEdge/message.txt");
	messageFile << email->m_message;
	messageFile.close();

	sprintf(piece, " %s", email->m_sender);
	strcat(command, piece);
	sprintf(piece, " %s", email->m_password);
	sprintf(piece, " %s", email->m_receiver);
	sprintf(piece, " %s", "C:\\Allevate\\Face-Searcher\\AureusEdge\\message.txt");

	printf("command: %s\n", command);
	system(command);
}