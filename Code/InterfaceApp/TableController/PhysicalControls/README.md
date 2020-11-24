# The Physical Table Controller

Is the python controller for the table.

The pixel assignment is a dictionary that defines how the pixels are set up on the table.
The key defines the physical location *the position (pos)* '(row, col)' that a pixel is on. The value is defined as '(node_id, pixel_id)'.

The 'update_pixel(pos, height, color)' function takes in a position of a pixel and the height and color as input. This updates the table on the code side. The data has not yet been sent to the physical table yet.

We can change the physical table, by sending it an updated version of the table through either 'send_pixel_data(pos)' which sends only a single pixel amount of data, 'send_node_data(node_id)' which sends the data of an entire node, or 'send_table_data' which sends data for the entire table.

## Use

The code should be used as a library that you import into you're own code. If you run the code as is, it will assume a single node configuration of the table, and will constantly send table data and display any returned information.
