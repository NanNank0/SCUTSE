`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2026/04/26 13:55:31
// Design Name: 
// Module Name: seven_seg_decoder
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


module seven_seg_decoder (
    input wire clk,
    input wire rst,
    input wire [15:0] sum,
    input wire [2:0] state,
    output reg [6:0] seg,
    output reg [3:0] an
);

    reg [1:0] digit_sel;
    reg [14:0] refresh_cnt;
    reg [3:0] digit_val;
    
    // 刷新计数器，用于数码管动态扫描
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            refresh_cnt <= 15'd0;
            digit_sel <= 2'd0;
        end else begin
            if (refresh_cnt == 15'd24999) begin
                refresh_cnt <= 15'd0;
                digit_sel <= digit_sel + 2'd1;
            end else begin
                refresh_cnt <= refresh_cnt + 15'd1;
            end
        end
    end
    
    // 选择当前要显示的位
    always @(*) begin
        case (digit_sel)
            2'd0: begin
                digit_val = sum[3:0];
                an = 4'b1110;
            end
            2'd1: begin
                digit_val = sum[7:4];
                an = 4'b1101;
            end
            2'd2: begin
                digit_val = sum[11:8];
                an = 4'b1011;
            end
            2'd3: begin
                digit_val = sum[15:12];
                an = 4'b0111;
            end
            default: begin
                digit_val = 4'd0;
                an = 4'b1111;
            end
        endcase
    end
    
    // 七段译码（共阳极）
    always @(*) begin
        case (digit_val)
            4'h0: seg = 7'b1000000; // 0
            4'h1: seg = 7'b1111001; // 1
            4'h2: seg = 7'b0100100; // 2
            4'h3: seg = 7'b0110000; // 3
            4'h4: seg = 7'b0011001; // 4
            4'h5: seg = 7'b0010010; // 5
            4'h6: seg = 7'b0000010; // 6
            4'h7: seg = 7'b1111000; // 7
            4'h8: seg = 7'b0000000; // 8
            4'h9: seg = 7'b0010000; // 9
            4'ha: seg = 7'b0001000; // A
            4'hb: seg = 7'b0000011; // B
            4'hc: seg = 7'b1000110; // C
            4'hd: seg = 7'b0100001; // D
            4'he: seg = 7'b0000110; // E
            4'hf: seg = 7'b0001110; // F
            default: seg = 7'b1111111;
        endcase
    end

endmodule