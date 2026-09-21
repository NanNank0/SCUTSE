`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/04/20 17:30:37
// Design Name: 
// Module Name: Decoder
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

module Mod_3(
    input [15:0] A,
    output [1:0] dout
    );
    function [1:0]mod_3_for_nibble;
        input [3:0]x;
        begin
            case(x)
                4'b0000 : mod_3_for_nibble = 2'b00 ;
                4'b0001 : mod_3_for_nibble = 2'b01 ;
                4'b0010 : mod_3_for_nibble = 2'b10 ;
                4'b0011 : mod_3_for_nibble = 2'b00 ;
                4'b0100 : mod_3_for_nibble = 2'b01 ;
                4'b0101 : mod_3_for_nibble = 2'b10 ;
                4'b0110 : mod_3_for_nibble = 2'b00 ;
                4'b0111 : mod_3_for_nibble = 2'b01 ;
                4'b1000 : mod_3_for_nibble = 2'b10 ;
                4'b1001 : mod_3_for_nibble = 2'b00 ;
                4'b1010 : mod_3_for_nibble = 2'b01 ;
                4'b1011 : mod_3_for_nibble = 2'b10 ;
                4'b1100 : mod_3_for_nibble = 2'b00 ;
                4'b1101 : mod_3_for_nibble = 2'b01 ;
                4'b1110 : mod_3_for_nibble = 2'b10 ;
                4'b1111 : mod_3_for_nibble = 2'b00 ;
            endcase
        end
    endfunction
    
    wire [1:0] r1 = mod_3_for_nibble(A[3:0]);
    wire [1:0] r2 = mod_3_for_nibble(A[7:4]);
    wire [1:0] r3 = mod_3_for_nibble(A[11:8]);
    wire [1:0] r4 = mod_3_for_nibble(A[15:12]);
    wire [3:0] sum = r1 + r2 + r3 + r4;
    wire [1:0] dout = mod_3_for_nibble(sum[3:0]);

endmodule

module tb_mod();
reg [2:0] din;
wire [7:0] dout;
mod_3 
mod_3_dut (
 .din (A),
 .dout  (dout)
);

initial begin
    A = 0;  #10
    A = 1;  #10
    A = 2;  #10
    A = 3;  #10
    A = 1000;   #10
    A = 1001;   #10
    A = 1002;   #10
    $finish
end


endmodule


module sram_128x8 (
    input         clk,          // Clock signal
    input         we,           // Write enable (active high)
    input  [6:0]  addr,         // Address (2^7 = 128)
    input  [7:0]  din,          // Data input
    output reg [7:0] dout       // Data output
);

    // 128x8 memory array
    reg [7:0] memory [0:127];
    
    // Initialize memory from file
    initial begin
        $readmemh("D:/work/verilog/labs/mem_acc/rams_init_file.data", memory);
    end
    
    // Write and read operations
    always @(posedge clk) begin
        if (we) begin
            memory[addr] <= din;
        end
        dout <= memory[addr];
    end

endmodule

module counter_7bit (
    input         clk,    // Clock signal
    input         rst,    // Asynchronous reset, active high
    input         en,     // Count enable
    input         clr,    // Clear signal
    output reg [6:0] cnt, // Counter value, 7-bit
    output        full    // Full indicator signal
);
    assign full = (cnt == 7'd127) ? 1'b1 : 1'b0;
    
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            cnt <= 7'd0; 
        end
        else if (clr) begin
            cnt <= 7'd0;  
        end
        else if (en) begin
            if (cnt == 7'd127) begin
                cnt <= 7'd0; 
            end
            else begin
                cnt <= cnt + 7'd1;
            end
        end
        else begin
            cnt <= cnt; 
        end
    end

endmodule

// Moore类型状态机
// 输入信号：clk, rst, full
// 输出信号：cut_en, cnt_clr, mem_we, acc_en, state
// 状态 ：IDLE等待复位信号，READ从SRAM读取数据，ACC累加，DONE完成累加并输出

module mem_acc (
    input         clk,
    input         rst, 
    output [6:0]  seg,
    output [3:0]  an 
);
    wire [6:0]  addr;
    wire [7:0]  data;
    wire        full;
    wire        cnt_en, cnt_clr;
    wire [15:0] sum;
    
    reg [1:0] state;
    localparam S_IDLE = 2'b00,
               S_READ = 2'b01,
               S_ACC  = 2'b10,
               S_DONE = 2'b11;
    
    counter_7bit counter (
        .clk(clk),
        .rst(rst),
        .en(cnt_en),
        .clr(cnt_clr),
        .cnt(addr),
        .full(full)
    );
    
    sram_128x8 memory (
        .clk(clk),
        .we(1'b0),         
        .addr(addr),
        .din(8'b0),
        .dout(data)
    );
    
    reg [15:0] acc;
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            acc <= 16'd0;
        end
        else if (state == S_ACC) begin
            acc <= acc + data;
        end
    end
    
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            state <= S_IDLE;
        end
        else begin
            case (state)
                S_IDLE: state <= S_READ;
                S_READ: state <= S_ACC;
                S_ACC: begin
                    if (full) state <= S_DONE;
                    else state <= S_READ;
                end
                S_DONE: state <= S_DONE;
                default: state <= S_IDLE;
            endcase
        end
    end
    
    assign cnt_en = (state == S_READ);
    assign cnt_clr = (state == S_IDLE);
    
endmodule