The Logic of our homework is to load the sensors and base stations both as struct POINT.
Locally we successfully parse the base stations txt file.

We’ve created structures: 

1.Point: We load the base stations as points, the load point function convert a string of reachable list 
to an array of Points. Free point and free points functions handle the memory issue. 

2.DataMessage: We read in and parse the data message file and extract the id, next, destination id, hotlist, the length of hoplist. Also we have a convert message function which convert parsed data message to string in the buffer for output purpose. Free DataMessage function handle the memory issue. 

3.BaseStation: We read in and parse the base stations txt file into an array of BaseStations. the find station function takes in an ID and an array of base stations to find the station with the intended ID.

4.StructClient: We initialize client, get the max_socket and selectonsockets (just like we did in previous homeworks). The free client function set a client to the initial status to handle memory issue.

we handle SENDDATA in the server 
     // return 0 if control --> client
    // return 1 if base --> client
    // return 2 if base --> base
    // return 3 if cannot find next legal client

we used our code in lab6 in the client.c to get the ip address of the host.