1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client knows when a command's output is fully received by looking for the EOF character (0x04), which signals the end of the message. Since data might arrive in chunks, the client keeps calling recv() in a loop until it gets the EOF character. This way, even if the message is split up, the client can put it back together before using it.

2. This week's lecture on TCP explains that it is a reliable stream protocol rather than a message-oriented one. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

Since TCP is a stream-based protocol and doesn’t keep message boundaries, a networked shell protocol should use a special delimiter (like a null byte \0 for commands and EOF 0x04 for responses) to mark the start and end of a message. The client and server must keep reading data in a loop until they detect the full message. If this isn’t handled correctly, commands can get cut off, mixed together, or processed incorrectly, causing unexpected behavior or errors in execution.


3. Describe the general differences between stateful and stateless protocols.

A stateful protocol keeps track of past interactions, meaning the server remembers previous requests and can use that information for future ones. A stateless protocol treats each request as independent, with no memory of past interactions. Stateful protocols usually require more resources but allow for persistent connections, while stateless ones are simpler and more scalable but may require extra data to be sent in each request.

4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used when speed is more important than reliability, like in video streaming, online gaming, or VoIP, where a few lost packets won’t ruin the experience.

5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides sockets as the main abstraction for network communication, allowing applications to send and receive data over networks. Sockets support both TCP and UDP communication, enabling processes to interact over local or remote networks.