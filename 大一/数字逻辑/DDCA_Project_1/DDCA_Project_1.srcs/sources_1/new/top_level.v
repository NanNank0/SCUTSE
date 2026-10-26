`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/04/26 13:55:30
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

module top_level (
    input wire clk,
    input wire rst,
    output wire [6:0] seg,
    output wire [3:0] an
);

    wire [6:0] addr;
    wire [7:0] data_out;
    wire [15:0] sum;
    wire [2:0] state;
    
    // 例化存储器
    ram_128x8 u_ram (
        .clk(clk),
        .addr(addr),
        .dout(data_out)
    );
    
    // 例化累加器
    accumulator u_acc (
        .clk(clk),
        .rst(rst),
        .addr(addr),
        .data_in(data_out),
        .sum(sum),
        .state(state)
    );
    
    // 例化七段译码器
    seven_seg_decoder u_seg (
        .clk(clk),
        .rst(rst),
        .sum(sum),
        .state(state),
        .seg(seg),
        .an(an)
    );

endmodule
