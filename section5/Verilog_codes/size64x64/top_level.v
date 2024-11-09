`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 30.10.2024 12:14:24
// Design Name: 
// Module Name: top_level
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


module top_level(
input       clk,
input       rst,
input       ap_start,
output [7:0] compressed_data,
output [31:0] compressed_size,
output      compressed_data_ap_vld,
output      ap_ready,
output      ap_done,
output      ap_idle
    );
    
wire  [11:0]  addra;
wire  [7:0]   douta;
wire          ena, enb,ap_start;    
//----------- Begin Cut here for INSTANTIATION Template ---// INST_TAG
wire  [31:0]  size;
wire          ap_done, ap_ready, ap_idle;

compress_deflate_image_0 dut (
  .image_data_ce0(ena),                  // output wire image_data_ce0
  .compressed_data_ap_vld(compressed_data_ap_vld),  // output wire compressed_data_ap_vld
  .compressed_size_ap_vld(compressed_size_ap_vld),  // output wire compressed_size_ap_vld
  .ap_clk(clk),                                  // input wire ap_clk
  .ap_rst(rst),                                  // input wire ap_rst
  .ap_start(ap_start),                              // input wire ap_start
  .ap_done(ap_done),                                // output wire ap_done
  .ap_idle(ap_idle),                                // output wire ap_idle
  .ap_ready(ap_ready),                              // output wire ap_ready
  .image_data_address0(addra),        // output wire [11 : 0] image_data_address0
  .image_data_q0(douta),                    // input wire [7 : 0] image_data_q0
  .width(size),                                    // input wire [31 : 0] width
  .height(size),                                  // input wire [31 : 0] height
  .compressed_data(compressed_data),                // output wire [7 : 0] compressed_data
  .compressed_size(compressed_size)                // output wire [31 : 0] compressed_size
);

blk_mem_gen_0 mem (
  .clka(clk),    // input wire clka
  .ena(ena),      // input wire ena
  .addra(addra),  // input wire [11 : 0] addra
  .douta(douta)  // output wire [7 : 0] douta
);

assign size = 32'd64;
   
endmodule
