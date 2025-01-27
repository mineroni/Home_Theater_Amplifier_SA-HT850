#ifndef WEBPAGEADVANCED_H
#define WEBPAGEADVANCED_H

const char * advancedPage = R"rawliteral(
<!DOCTYPE html>
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Advanced Volume Controls</title>
    <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
    <style>
        .volumeBar {
            width: 70%;
            height: 30px;
            background: #007bff;
            border-radius: 15px;
            position: relative;
        }
    
        .volumeFill {
            height: 100%;
            background: #00d4ff;
            border-radius: 15px;
            width: 50%; /* Default to 50% for volume 50 */
        }
    
        .volumeText {
            font-size: 2rem;
            font-weight: bold;
            color: #ffcc00;
        }
    
        .mutedStatus {
            color: red;
            visibility: hidden;
            font-size: 1.5rem;
            font-weight: bold;
        }
    </style>
    </head>
    <body class="bg-dark text-light">
        <button onclick="window.history.back();" class="btn btn-primary mb-2 mt-3">Go Back</button>
        <div class="container text-center py4">
            <h1 class="mb-4">Advanced Volume Controls</h1>
    
            <!-- Volume Control -->
            <div class="volumeText" id="volume">Volume: 50%</div>
            <h4 class="mutedStatus" id="muted">Muted</h4>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <div class="volumeBar" id="volumeBar">
                    <div class="volumeFill" id="volumeFill"></div>
                </div>
            </div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <button class="btn btn-secondary" id="volumeDownBtn">Volume Down</button>
                <button class="btn btn-secondary" id="volumeUpBtn">Volume Up</button>
            </div>
    
            <!-- Surround offset Control -->
            <div class="volumeText" id="surround">Surround offset: 0</div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <div class="volumeBar" id="surroundBar">
                    <div class="volumeFill" id="surroundFill"></div>
                </div>
            </div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <button class="btn btn-secondary" id="surroundDownBtn">Surround Offset Down</button>
                <button class="btn btn-secondary" id="surroundUpBtn">Surround Offset Up</button>
            </div>
    
            <!-- Subwoofer offset Control -->
            <div class="volumeText" id="subwoofer">Subwoofer offset: 0</div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <div class="volumeBar" id="subwooferBar">
                    <div class="volumeFill" id="subwooferFill"></div>
                </div>
            </div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <button class="btn btn-secondary" id="subwooferDownBtn">Subwoofer Offset Down</button>
                <button class="btn btn-secondary" id="subwooferUpBtn">Subwoofer Offset Up</button>
            </div>
    
            <!-- Center offset Control -->
            <div class="volumeText" id="center">Center offset: 0</div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <div class="volumeBar" id="centerBar">
                    <div class="volumeFill" id="centerFill"></div>
                </div>
            </div>
    
            <div class="d-flex justify-content-center gap-3 mb-3">
                <button class="btn btn-secondary" id="centerDownBtn">Center Offset Down</button>
                <button class="btn btn-secondary" id="centerUpBtn">Center Offset Up</button>
            </div>

            
            <button onclick="fetch('/reset-offsets').catch(console.error);" class="btn btn-primary mt-4">Reset all offset value</button>
        </div>
    </body>
    

    <script>
        const volumeFill = document.getElementById('volumeFill');
        const volumeText = document.getElementById('volume');
        const mutedStatus = document.getElementById('muted');
        const surroundFill = document.getElementById('surroundFill');
        const surroundText = document.getElementById('surround');
        const subwooferFill = document.getElementById('subwooferFill');
        const subwooferText = document.getElementById('subwoofer');
        const centerFill = document.getElementById('centerFill');
        const centerText = document.getElementById('center');

        function updateUI(data) {
            volumeFill.style.width = `${data.volume}%`;
            volumeText.textContent = `Volume: ${data.volume}%`;
            mutedStatus.style.visibility = data.muted ? 'visible' : 'hidden';

            surroundFill.style.width = `${(data.surround_offset+10)*5}%`;
            surroundText.textContent = `Subwoofer offset: ${data.surround_offset}`;

            subwooferFill.style.width = `${(data.subwoofer_offset+10)*5}%`;
            subwooferText.textContent = `Subwoofer offset: ${data.subwoofer_offset}`;

            centerFill.style.width = `${(data.center_offset+10)*5}%`;
            centerText.textContent = `Center offset: ${data.center_offset}`;
        }

        document.getElementById('volumeDownBtn').addEventListener('click', () => {
            fetch('/add-volume', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'level=-5'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        document.getElementById('volumeUpBtn').addEventListener('click', () => {
            fetch('/add-volume', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'level=5'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        // The same for surround, subwoofer, and center buttons
        document.getElementById('surroundDownBtn').addEventListener('click', () => {
            fetch('/add-offsetSurround', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'offset=-1'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        document.getElementById('surroundUpBtn').addEventListener('click', () => {
            fetch('/add-offsetSurround', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'offset=1'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        document.getElementById('subwooferDownBtn').addEventListener('click', () => {
            fetch('/add-offsetSubwoofer', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'offset=-1'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        document.getElementById('subwooferUpBtn').addEventListener('click', () => {
            fetch('/add-offsetSubwoofer', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'offset=1'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        document.getElementById('centerDownBtn').addEventListener('click', () => {
            fetch('/add-offsetCenter', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'offset=-1'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        document.getElementById('centerUpBtn').addEventListener('click', () => {
            fetch('/add-offsetCenter', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/x-www-form-urlencoded'
                },
                body: 'offset=1'
            })
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        });

        function fetchValues() {
            fetch('/get-status')
            .then(response => response.json())
            .then(data => updateUI(data))
            .catch(console.error);
        }

        setInterval(fetchValues, 500);
        fetchValues();
    </script>
</body>
)rawliteral";

#endif // WEBPAGEADVANCED_H