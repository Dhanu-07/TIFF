`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 30.10.2024 14:38:04
// Design Name: 
// Module Name: test_TB
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module test_TB;




top_level   DUT( clk, rst, ap_start, compressed_data, compressed_size, compressed_data_ap_vld,
                 ap_ready, ap_done, ap_idle);


reg  clk, rst, ap_start;
wire [7:0] compressed_data;
wire [31:0] compressed_size;
integer   f1;

initial  begin
f1 = $fopen("comp_image.txt","w");
rst     = 1;
clk     = 0;

#200
rst     = 0;

end

always
#5 clk = ~clk;

always @ (ap_done)
    $finish;


always @ (posedge clk)
    if (rst)
      ap_start = 0;   
    else 
      ap_start = ap_idle? 1 : ap_start;
      
always @ ( posedge compressed_data_ap_vld)
 $fwrite(f1, "%b\n", compressed_data);

endmodule
