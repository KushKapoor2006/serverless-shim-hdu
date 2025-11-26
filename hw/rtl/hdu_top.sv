`timescale 1ns / 1ps
import hdu_pkg::*;

module hdu_top #(
    parameter int TABLE_SIZE = 16
)(
    input  logic                   clk,
    input  logic                   rst_n,

    // AXI-Stream Input
    input  logic [DATA_WIDTH-1:0]  s_axis_tdata,
    input  logic                   s_axis_tlast,
    input  logic                   s_axis_tvalid,
    output logic                   s_axis_tready,

    // Test harness injection (for C++ harness)
    input  logic                   tb_inject_valid,
    input  logic [TOKEN_WIDTH-1:0] tb_inject_token,
    input  logic [FUNC_ID_WIDTH-1:0] tb_inject_func,

    // Dispatch Interface
    output logic [SLOT_ID_WIDTH-1:0] dispatch_slot,
    output logic                     dispatch_valid,
    input  logic [SLOT_ID_WIDTH-1:0] compute_done_slot,
    input  logic                     compute_done_valid,

    // Config & Host
    input  logic [$clog2(TABLE_SIZE)-1:0] cfg_addr,
    input  logic                          cfg_wr_en,
    input  logic [FUNC_ID_WIDTH-1:0]      cfg_func_id,
    input  logic [TOKEN_WIDTH-1:0]        cfg_token,
    
    output logic                     host_irq,
    input  logic                     host_ready,
    
    // Telemetry Bus
    output logic [31:0] status_allocs,
    output logic [31:0] status_fails,
    output logic [31:0] status_overflows
);

    // internal signals
    logic [FUNC_ID_WIDTH-1:0] prs_func_id, stg2_func_id;
    logic [TOKEN_WIDTH-1:0]   prs_token, stg2_token;
    logic                     prs_valid, prs_ready;

    logic                     auth_pass, auth_done, auth_ready;
    logic                     alloc_req_valid, alloc_req_ready;
    logic                     alloc_success, alloc_fail, alloc_valid_out;
    logic [SLOT_ID_WIDTH-1:0] alloc_slot_id;

    // parser
    header_parser u_parser (
        .clk(clk), .rst_n(rst_n),
        .s_axis_tdata(s_axis_tdata), .s_axis_tlast(s_axis_tlast),
        .s_axis_tvalid(s_axis_tvalid), .s_axis_tready(s_axis_tready),
        .out_func_id(prs_func_id), .out_token(prs_token),
        .out_valid(prs_valid), .out_ready(prs_ready),
        .tb_inject_valid(tb_inject_valid),
        .tb_inject_token(tb_inject_token),
        .tb_inject_func(tb_inject_func)
    );

    // pipeline capture to align with auth latency
    always_ff @(posedge clk or negedge rst_n) begin
        if (!rst_n) begin
            stg2_func_id <= '0;
            stg2_token   <= '0;
        end else if (prs_valid && prs_ready) begin
            stg2_func_id <= prs_func_id;
            stg2_token   <= prs_token;
        end
    end

    assign prs_ready = auth_ready && alloc_req_ready;

    auth_cam #(.TABLE_SIZE(TABLE_SIZE)) u_cam (
        .clk(clk), .rst_n(rst_n),
        .cfg_addr(cfg_addr), .cfg_wr_en(cfg_wr_en),
        .cfg_func_id(cfg_func_id), .cfg_token(cfg_token),
        .lookup_func_id(prs_func_id), .lookup_token(prs_token),
        .lookup_valid(prs_valid && prs_ready),
        .lookup_ready(auth_ready),
        .auth_pass(auth_pass), .auth_done(auth_done)
    );

    assign alloc_req_valid = auth_done && auth_pass;

    slot_allocator u_alloc (
        .clk(clk), .rst_n(rst_n),
        .req_valid(alloc_req_valid), .req_ready(alloc_req_ready),
        .free_slot_id(compute_done_slot), .free_en(compute_done_valid),
        .alloc_slot_id(alloc_slot_id), 
        .alloc_success(alloc_success), .alloc_fail(alloc_fail),
        .alloc_valid(alloc_valid_out),
        .cnt_allocations(status_allocs), .cnt_failures(status_fails)
    );

    assign dispatch_valid = alloc_valid_out && alloc_success;
    assign dispatch_slot  = alloc_slot_id;

    logic trigger_fallback;
    assign trigger_fallback = (auth_done && !auth_pass) || (alloc_valid_out && alloc_fail);

    fallback_bridge u_fallback (
        .clk(clk), .rst_n(rst_n),
        .fail_valid(trigger_fallback),
        .fail_func_id(stg2_func_id),
        .fail_token(stg2_token),
        .host_ready(host_ready),
        .irq_valid(host_irq),
        .irq_data(), // optional
        .cnt_overflows(status_overflows)
    );

endmodule
