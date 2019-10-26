"# client_server_chat" 

A simple clien-server chat code to communicate.
run client and server in different terminals in same computer.

Client:

/quit
Logout from the server, terminates the program.

/login UserID UserPassword IPAddress Port#
Login to the server.
If account is on the client list, Logged IN.
If not, NOT Logged IN.

/createuser UserID UserPassword IPAddress Port#
Add(Create) a new account to the client list.
If same UserID exists, new account will be refused by the server.
If successful, client will be automatically logged in to the server with the new account.

*AFTER LOGGING IN*

/createsession SessionID
Create a new session with SessionID.
If same SessionID exists, SessionID will be refused by the server.
If successful, UserID will be added to the SessioID session.

/joinsession SessionID
Join to an existing session with SessionID.
If session doesn't exist, server will refuse.
If successful, UserID will be added to the SessioID session.

/leavesession SessionID
Leave the session with SessionID.
If UserID is not in the SessionID, UserID will be notified.
If successful, client will move to the SessionID added latest.

/changesession SessionID
Change the session client is currently in to SessionID.
If successful, client will be in the SessionID.

/invite InvitingUserID SessionIDclientInvitingTo
Invite InvitingUserID currently logged in, to SessionIDclientInvitingTo.
Client has to be joined in SessionIDclientInvitingTo in order to invite InvitingUserID.
InvitingUserID will receive an Invited Message, and gives an response(y/n).


/logout
Logout from the server, but program not terminated.

/list
List all the available UserIDs and SessionIDs.

/history SessionID
Client asks for the history of the conversation on SessionID.
Prints out all the conversation.
