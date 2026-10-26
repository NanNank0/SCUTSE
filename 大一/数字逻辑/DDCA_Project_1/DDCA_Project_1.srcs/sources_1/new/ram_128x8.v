`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/04/26 13:55:31
// Design Name: 
// Module Name: ram_128x8
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


module ram_128x8 (
    input wire clk,
    input wire [6:0] addr,
    output reg [7:0] dout
);

    reg [7:0] mem [0:127];
    
    // 从文件初始化存储器
    initial begin
        $readmemh("rams_init_file.data", mem);
    end
    
    // 同步读操作
    always @(posedge clk) begin
        dout <= mem[addr];
    end

endmodule
