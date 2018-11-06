class Api::V1::VariablesController < Api::V1::BaseController

  def index
    render json: Variable.find_by(name: params[:name])
  end

  def update
    variable = Variable.find(params[:id])
    variable.update_attributes(variable_params)
    render json: variable
  end

  def variable_params
    params.require(:variable).permit(:id, :name, :status, :value)
  end

end
