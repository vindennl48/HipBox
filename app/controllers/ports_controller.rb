class PortsController < ApplicationController
  before_action :set_port, only: [:edit, :update]

  def index
    @ports       = Port.all.order("id")
    @port_groups = PortGroup.all.order("id")
  end

  def edit
  end

  def update
    respond_to do |format|
      if @port.update(port_params)
        format.html { redirect_to ports_path, notice: 'port was successfully updated.' }
        format.json { render :show, status: :ok, location: @port }
      else
        format.html { render :edit }
        format.json { render json: @port.errors, status: :unprocessable_entity }
      end
    end
  end

  private

  def set_port
    @port = Port.find(params[:id])
  end

  def port_params
    params.fetch(:port, {}).permit(:id, :name, :io, :path, :pan, :port_group_id)
  end

end
