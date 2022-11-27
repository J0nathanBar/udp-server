import React from 'react'
import './displayPanel.css'
const DisplayPanel = (props) => {
  return (
    <input
    value={props.text}
    readOnly={true}></input>
  )
}

export default DisplayPanel
