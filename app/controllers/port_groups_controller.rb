class PortGroupsController < ApplicationController
  before_action :set_port_group, only: [:edit, :update, :destroy]

  def new
    @port_group = PortGroup.find_or_create_by(name: "new group", io: params['io'])
    redirect_to ports_path
  end

  def edit
  end

  def update
    respond_to do |format|
      if @port_group.update(port_group_params)
        format.html { redirect_to ports_path, notice: 'Port group was successfully updated.' }
        format.json { render :show, status: :ok, location: @port_group }
      else
        format.html { render :edit }
        format.json { render json: @port_group.errors, status: :unprocessable_entity }
      end
    end
  end

  def destroy
    @port_group.destroy
    respond_to do |format|
      format.html { redirect_to ports_path, notice: 'Port group was successfully destroyed.' }
      format.json { head :no_content }
    end
  end

  private

  def set_port_group
    @port_group = PortGroup.find(params[:id])
  end

  def port_group_params
    params.fetch(:port_group, {}).permit(:id, :name, :io, :is_global, :is_global_mute, :is_record, :user_id)
  end
end
