$(function () {
    $(".disk-btn").click((e)=>{
        //const disk = e.targe.value
        console.log(e.target.value)
        $.ajax({
            url:"/delete",
            type:"POST",
            contentType: "application/json",
            data: JSON.stringify({"disk":e.target.value})
        });

    })

})
