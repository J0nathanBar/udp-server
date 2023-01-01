import React, { useState } from 'react';
import './FileForm.css';

function FileForm() {
  const [mode, setMode] = useState('receive');
  const [selectedFile, setSelectedFile] = useState(null);

  const handleFileChange = (event) => setSelectedFile(event.target.files[0]);

  const handleSubmit = (event) => {
    event.preventDefault();
    if (!selectedFile) return;
    fetch('http://localhost:5000/api', {
      method: 'POST',
      headers: { 'Content-Type': 'application/json' },
      body: JSON.stringify({ mode, path: selectedFile.fullPath })
    });
  };

  return (
    <form onSubmit={handleSubmit} className="form-container">
      <label htmlFor="file-input">
        <input type="file" id="file-input" onChange={handleFileChange} />
        Select file
      </label>
      {selectedFile ? (
        <div className="selected-file">Selected file: {selectedFile.path}</div>
      ) : (
        <div className="selected-file">No file selected</div>
      )}
      <label>
        <input type="radio" name="mode" value="receive" checked={mode === 'receive'} onChange={(event) => setMode(event.target.value)} />
        Receive data
      </label>
      <label>
        <input type="radio" name="mode" value="transmit" checked={mode === 'transmit'} onChange={(event) => setMode(event.target.value)} />
        Transmit data
      </label>
      <button type="submit">Submit</button>
    </form>
  );
}

export default FileForm;

