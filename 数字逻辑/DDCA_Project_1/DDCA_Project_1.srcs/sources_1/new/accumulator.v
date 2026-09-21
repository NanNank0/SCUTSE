`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/04/26 13:55:31
// Design Name: 
// Module Name: accumulator
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


module accumulator (
    input wire clk,
    input wire rst,
    output reg [6:0] addr,
    input wire [7:0] data_in,
    output reg [15:0] sum,
    output reg [2:0] state
);

    // 状态编码
    localparam IDLE     = 3'b000;
    localparam READ     = 3'b001;
    localparam ACCUM    = 3'b010;
    localparam DONE     = 3'b011;
    
    reg [6:0] cnt;
    
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            addr <= 7'd0;
            sum <= 16'd0;
            cnt <= 7'd0;
            state <= IDLE;
        end else begin
            case (state)
                IDLE: begin
                    addr <= 7'd0;
                    sum <= 16'd0;
                    cnt <= 7'd0;
                    state <= READ;
                end
                
                READ: begin
                    state <= ACCUM;
                end
                
                ACCUM: begin
                    // 累加当前读出的数据
                    sum <= sum + {8'd0, data_in};
                    
                    if (cnt == 7'd127) begin
                        state <= DONE;
                    end else begin
                        cnt <= cnt + 7'd1;
                        addr <= addr + 7'd1;
                        state <= READ;
                    end
                end
                
                DONE: begin
                    // 保持结果，不再变化
                    state <= DONE;
                end
                
                default: begin
                    state <= IDLE;
                end
            endcase
        end
    end

endmodule
