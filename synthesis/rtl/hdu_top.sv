`timescale 1ns/1ps
`include "hdu_defs.svh"

module hdu_top #(
    parameter TABLE_SIZE = 16
)(
    input logic clk,
    input logic rst_n,

    // AXIS input
    input  logic [`HDU_DATA_WIDTH-1:0] s_axis_tdata,
    input  logic                       s_axis_tvalid,
    output logic                       s_axis_tready,

    // Dispatch output
    output logic [`HDU_SLOT_ID_WIDTH-1:0] dispatch_slot,
    output logic                           dispatch_valid,

    // Slot free
    input logic [`HDU_SLOT_ID_WIDTH-1:0] compute_done_slot,
    input logic                           compute_done_valid,

    // CONFIG
    input logic [$clog2(TABLE_SIZE)-1:0] cfg_addr,
    input logic                           cfg_wr_en,
    input logic [`HDU_FUNC_ID_WIDTH-1:0] cfg_func_id,
    input logic [`HDU_TOKEN_WIDTH-1:0]   cfg_token,

    // Host
    input logic host_ready,
    output logic host_irq
);

    logic req_valid;
    logic [`HDU_FUNC_ID_WIDTH-1:0] req_func;
    logic [`HDU_TOKEN_WIDTH-1:0]   req_token;

    assign s_axis_tready = 1'b1;

    always_comb begin
        req_valid = s_axis_tvalid;
        req_func  = s_axis_tdata[`HDU_FUNC_TOP  : `HDU_FUNC_BOT];
        req_token = s_axis_tdata[`HDU_TOKEN_TOP : `HDU_TOKEN_BOT];
    end

    // ---------------- AUTH CAM ----------------
    logic cam_ready, auth_pass, auth_done;

    auth_cam #(.TABLE_SIZE(TABLE_SIZE)) CAM (
        .clk(clk), .rst_n(rst_n),
        .cfg_addr(cfg_addr), .cfg_wr_en(cfg_wr_en),
        .cfg_func_id(cfg_func_id), .cfg_token(cfg_token),
        .lookup_func_id(req_func), .lookup_token(req_token),
        .lookup_valid(req_valid), .lookup_ready(cam_ready),
        .auth_pass(auth_pass), .auth_done(auth_done)
    );

    // ---------------- ALLOCATOR ----------------
    logic alloc_valid, alloc_success, alloc_fail;
    logic [`HDU_SLOT_ID_WIDTH-1:0] alloc_slot_id;

    slot_allocator ALLOC (
        .clk(clk), .rst_n(rst_n),
        .req_valid(auth_done & auth_pass),
        .req_ready(),
        .free_slot_id(compute_done_slot),
        .free_en(compute_done_valid),
        .alloc_slot_id(alloc_slot_id),
        .alloc_success(alloc_success),
        .alloc_fail(alloc_fail),
        .alloc_valid(alloc_valid)
    );

    assign dispatch_slot  = alloc_slot_id;
    assign dispatch_valid = alloc_valid & alloc_success;

    // ---------------- FALLBACK ----------------
    fallback_bridge FB (
        .clk(clk), .rst_n(rst_n),
        .fail_valid(auth_done & ~auth_pass),
        .fail_func_id(req_func),
        .fail_token(req_token),
        .host_ready(host_ready),
        .irq_valid(host_irq),
        .irq_data()
    );

endmodule
