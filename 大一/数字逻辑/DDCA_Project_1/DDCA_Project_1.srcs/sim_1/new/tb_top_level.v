`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/04/26 13:59:05
// Design Name: 
// Module Name: tb_top_level
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


module tb_top_level;

    reg clk;
    reg rst;
    wire [6:0] seg;
    wire [3:0] an;
    
    // 内部信号用于观察
    wire [6:0] addr;
    wire [7:0] data_out;
    wire [15:0] sum;
    wire [2:0] state;
    
    // 例化顶层模块
    top_level u_top (
        .clk(clk),
        .rst(rst),
        .seg(seg),
        .an(an)
    );
    
    // 通过层次路径访问内部信号
    assign addr = u_top.u_acc.addr;
    assign data_out = u_top.u_ram.dout;
    assign sum = u_top.u_acc.sum;
    assign state = u_top.u_acc.state;
    
    // 100MHz时钟产生
    initial begin
        clk = 0;
        forever #5 clk = ~clk;  // 周期10ns = 100MHz
    end
    
    // 复位信号：第5ns有效，15ns结束
    initial begin
        rst = 0;
        #5 rst = 1;
        #10 rst = 0;
    end
    
    // 波形转储
    initial begin
        $dumpfile("wave.vcd");
        $dumpvars(0, tb_top_level);
    end
    
    // 140个周期后结束仿真
    initial begin
        #1400 $finish;  // 140 * 10ns = 1400ns
    end
    
    // 监控输出
    initial begin
        $monitor("Time=%0t ns | clk=%b rst=%b addr=%d data=%h state=%d sum=%h",
                 $time, clk, rst, addr, data_out, state, sum);
    end

endmodule
