function chartCreator(id, taken, free) {
    const ctx = document.getElementById(id);
    const data = {
        labels: ["Occupied Gb", "Free Gb"],
        datasets: [{
        data: [taken, free],
        backgroundColor: [
            'rgb(255, 99, 132)',
            'rgb(54, 162, 235)',
        ],
        hoverOffset: 4
        }]
    };
    const myChart = new Chart(ctx, {
        type: 'doughnut',
        data: data,
        options: {
            layout: {
                padding: 50
            },
            responsive: false
        }
    });
}


let numberOfDiscs = 5;
const chartDiv = document.getElementById("canvas-div");

for (let i = 0; i < numberOfDiscs; i++) {
    const chartDiv2 = document.createElement("div");
    const newChart = document.createElement("canvas");
    const chartTxt = document.createElement("p");
    chartTxt.className = "diskNo";
    chartTxt.textContent = `Disk ${i}`;
    newChart.id = `myCanvas${i}`
    newChart.width = 400;
    newChart.height = 400;
    newChart.padding = 50;
    chartDiv2.style.position = "relative" 
    chartDiv2.appendChild(chartTxt);
    chartDiv2.appendChild(newChart);
    chartDiv.appendChild(chartDiv2);
    let taken = 1000 + Math.floor(Math.random() * 50);
    let free =  500 + Math.floor(Math.random() * 50);
    chartCreator(`myCanvas${i}`, taken, free);
}