const numDisks = 5;
const container = document.getElementById("disk-div")
const desc = document.getElementById("disk-desc")
desc.style.whiteSpace = "pre";

let disksWorking = [1, 1, 1, 1, 1]
let working = 0;
let broken = 0;
let recovering = 0;

for (let j = 0; j < errs.length; j++) {
    disksWorking[parseInt(errs[j])] = 0;
}

console.log(disksWorking)


for (let j = 0; j < numDisks; j++) {
    if (disksWorking[j] === 1) working++;
    else if (disksWorking[j] === 0) broken++;
    else recovering++;

    if (broken === 0 && recovering === 0) {
        desc.textContent = "All disks operational"
    } else {
        desc.textContent = `${working} disks working.\n${broken} disks failed.\n${recovering} recovering.`;
    }
}

for (let i = 0; i < numDisks; i++) {
    const newDisk = document.createElement("div");
    newDisk.className = "disk";

    const diskNo = document.createElement("p");
    diskNo.className = "disk-name"

    diskNo.textContent = `Disk ${i}`;

    const yeetBtn = document.createElement("button");
    yeetBtn.className = "disk-btn";
    yeetBtn.value = `disk${i}`
    yeetBtn.textContent = "Simulate Failure"

    const icon = document.createElement("i")
    
    newDisk.appendChild(diskNo);
    newDisk.append(icon)
    newDisk.appendChild(yeetBtn);

    if (disksWorking[i] === 1) {
        icon.className = "fa-solid fa-check working"
        icon.style.color = "green";
    } else if (disksWorking[i] === 0){
        icon.className = "fa-solid fa-x working"
        icon.style.color = "red";
        const recoverBtn = document.createElement("button");
        recoverBtn.className = "disk-btn";
        recoverBtn.textContent = "Attempt recover"
        newDisk.appendChild(recoverBtn)
    } else {
        icon.className = "fa-solid fa-rotate working"
        icon.style.color = "blue";
    }

    container.appendChild(newDisk);

}