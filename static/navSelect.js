$(function(){
    const space = $("#space")
    const perf = $("#performance")
    const hist = $("#history")

    const capac = $("#capacity")
    const disk = $("#disks");

    $('.menu').css("display", "none");
    $('#data-show').css("display", "inline-block");

    $('.toggle-header').css("display", "none");
    $('#data').css("display", "inline-block");

    const overl = space.find('.squircle-overlay');
    overl.css("opacity", "100%");

    space.click(() => {
        $(".squircle-overlay").css("opacity", "0%");
        const overl = space.find('.squircle-overlay');
        overl.css("opacity", "100%");

        $('.toggle-header').css("display", "none");
        $('#data').css("display", "inline-block");
    });

    perf.click(() => {
        $(".squircle-overlay").css("opacity", "0%");
        const overl = perf.find('.squircle-overlay');
        overl.css("opacity", "100%");

        $('.toggle-header').css("display", "none");
        $('#perf-div').css("display", "inline-block");
    });

    hist.click(() => {
        $(".squircle-overlay").css("opacity", "0%");
        const overl = hist.find('.squircle-overlay');
        overl.css("opacity", "100%");

        $('.toggle-header').css("display", "none");
        $('#history-div').css("display", "inline-block");
    });

    capac.click(() => {
        $(".side-element").removeClass("selected");
        $("#capacity").addClass("selected");

        $('.menu').css("display", "none");
        $('#data-show').css("display", "inline-block");
    })

    disk.click(() => {
        $(".side-element").removeClass("selected");
        $("#disks").addClass("selected");

        $('.menu').css("display", "none");
        $('#disk-show').css("display", "inline-block");
    })
})