FROM python:3.13.0-slim-bookworm

# Create the virtual environment:
ENV VIRTUAL_ENV=/opt/venv

RUN python3 -m venv $VIRTUAL_ENV && adduser pythonuser
ENV PATH="$VIRTUAL_ENV/bin:$PATH" PYTHONUNBUFFERED=1

# This image needs 3 additional environment variables:
# - CONSUL_ADDRESS: The address where the Consul server is running.
# - CONSUL_TOKEN: The token for Consul authentication. This should be the servicediscovery token.
# - OPENPROVIDER_PASSWORD: The openprovider password for the user peroni.
# These variables should be passed when running the container, this can be done in the Nomad job

WORKDIR /opt/venv

# Install dependencies:
COPY main.py requirements.txt /opt/venv/

RUN pip install --no-cache-dir -r requirements.txt

USER pythonuser

# Let's goooo!!!
CMD ["python", "main.py"]
