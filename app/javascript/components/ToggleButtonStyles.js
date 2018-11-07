import styled from "styled-components"


let Button = styled.div`
  display:       grid;
  place-items:   center;
  width:         ${props => props.width};
  height:        ${props => props.height};
  ${props => props.mediaQuery}
  border-radius: 5px;
  font-size:     12px;
  border:        1px solid #777;
  box-shadow:    none;
  transition:    all 0.3s ease;

  ${props => props.isDisabled ? `
    color:  #444;
    border: 1px solid #444;
  ` : ''}

  ${props => props.isRemote ? `
    background-color: ${props.remoteColorActive};
  ` : ''}

  ${props => props.value ? `
    box-shadow:       inset 2px 2px 5px #222;
    color:            ${props.textColorActive};
    background-color: ${props.bgColorActive};
    transition:       all 0.3s ease;
  ` : `
    transition: all 0.3s ease;
  `}
`;

export default Button
