# Components of the network/project:

1. Nodes
   1. Next node to forward to, for every active VCID (connection)
2. Links between Nodes
   1. Propagation delay (ms)
   2. Capacity (Mbps)
      1. Ideally separate for both uplink and downlink (but initially both are same)
3. Central Controller
   1. Should know the overall topology of the network
   2. Should know the shortest (and second shortest) path between any two nodes
   3. State about the number of connections attempted, and successfull. Additionally, information about the active connections should be maintained
      1. For every connection: Conn. ID, Source, Destination, Path, VC ID, PathCost should be maintained


Two design choices are possible here (FINALISED TO GO WITH KACC DESIGN):
1. Know-all Centralised Controller (KACC) design
   1. Knows (or queries) all the capacities of all the links in the path and sets up the connection path (forwarding tables) accordingly
2. ~Intelligent router (IR) design~ 
   1. Routers themselves send connection setup messages to their next neighbours to set up the connection path


## Design implications

`routingfile` can be easily printed with KACC design, but with IR design, it needs to go through all the routes to find the path.
`forwardingfile` can be printed by querying the forwarding data of each node.
`pathsfile` can be generated using the central controller's data.

## Flow of the project

1. Command line parser to get all the meta information
2. Configuration object of the network
   1. topofilename
   2. routingfilename
   3. forwardingfilename
   4. pathsfilename
   5. hop/dist
   6. pessimistic/optimistic
3. Simulator object
   1. Reads the topo file and creates a central controller
   2. Asks the controller to compute the APSP and second APSP
   3. Gets the connection information from the topo file and sets up the connections
      1. Additionally, it keeps on appending the connection information to the `pathsfile`
   4. Gets the routing information from the controller and creates the routing file
   5. Gets the forwarding information for a given node and accumulates it to create the forwarding file (what to do here, should the controller handle this?)
      1. Probably the controller can return only one forwarding table and its the job of the simulator to accumulate all the tables
4. Central Controller
   1. Does not do file handling
   2. Has the following API
      1. Add node
      2. Add link
      3. Add connection
      4. Compute APSP (and second APSP)
      5. Get all routing paths
5. Node
   1. Has the following members
      1. Node ID
      2. Forwarding Table
         1. VC ID, in
         2. VC ID, out
         3. Node ID, in
         4. Node ID, out
      3. Neighbour node IDs and their link objects
   2. It should have the following API
      1. Add neighbour, link
      2. Add forwarding table entry
      3. (Not needed) lookup forwarding table entry given input node, VC ID
6. Link
   1. Has the following members
      1. Capacity
      2. Propagation delay
      3. Source node ID
      4. Destination node ID